import argparse
import os
import re
import sys

def processHLSL(input_file, in_entrypoint):
    # Process the intermediate clang output into valid HLSL
    # The data generated here is of the form:
    # { "entrypoint": {
    #        "raw_hlsl": <entrypoint specific HLSL>},
    #   "<name>_common": {
    #        "raw_hlsl": <Common HLSL for module>}
    module_name = os.path.splitext(os.path.basename(input_file))[0]
    common_name = module_name + "_common"

    # data_dict accumulates all HLSL information for all entrypoints in the module
    # The entry for common_name writes into *_common.h to be included by all entrypoints
    data_dict = {common_name: {"raw_hlsl": []}}
    current_entry = data_dict[common_name]
    num_uavs = num_srvs = num_cbvs = num_samplers = 0
    sampler_string = ""

    # Tracking braces to ensure common code can be present after an entry point
    num_braces = 0
    is_brace_monitoring_on = False

    # Read the intermediate file and process it
    with open(input_file, 'r') as clang_intermediate:
        for line_data in clang_intermediate:
            num_braces_inline = line_data.count('{') - line_data.count('}')

            # Sampler:
            # <name>__HLSL_SAMPLER;filter=<>,addressU=<>,addressV=<>,addressW=<>
            if "__HLSL_SAMPLER" in line_data:
                line_data = line_data.replace("__HLSL_SAMPLER", ":register(s{num_samplers})".format(num_samplers=num_samplers))
                line_data = line_data.replace("\n", "")
                # Add the sampler registration to the raw hlsl
                current_entry["raw_hlsl"].append(line_data.split(';')[0] + ";\n")
                sampler_details = line_data.split(';')[1]
                # Append the sampler details to the sampler string. This will be a part of Root Signature
                sampler_string += ",StaticSampler(s{num_samplers}, {sampler_details})".format(num_samplers=num_samplers, sampler_details=sampler_details)
                num_samplers += 1
                continue

            # Entrypoint:
            # Macro expansion adds __HLSL_ENTRYPOINT_FILE to entrypoint function name
            if "__HLSL_ENTRYPOINT_FILE_" in line_data:
                # This line contains all information about the arguments passed to the entrypoint
                # Break apart everything and create valid hlsl for it 
                for entrypoint_line in line_data.split(';'):
                    # Use the part which contain the entrypoint name to create a new entry in the data_dict
                    # Also populate the blocksize and shared memory data for the entrypoint
                    if "__HLSL_ENTRYPOINT_FILE_" in entrypoint_line:
                        entrypoint_name = entrypoint_line.replace("__HLSL_ENTRYPOINT_FILE_", "").strip()
                        # Create a new entry in the data_dict for the entrypoint
                        data_dict[entrypoint_name] = {
                            "raw_hlsl": [], 
                            "shared_decls": []}
                        current_entry = data_dict[entrypoint_name]
                        continue
                    # Pick on each argument that needs a register bound to it
                    elif "__HLSL_REGISTER" in entrypoint_line:
                        # This is a dynamic shared memory case. No need to register but add it to the list of shared decls
                        if "__HLSL_SHARED_MEMORY_SIZE" in entrypoint_line:
                            entrypoint_line = entrypoint_line.replace("__HLSL_REGISTER", "")
                            current_entry["shared_decls"].append(entrypoint_line + ";\n")
                            continue
                        # Handle constant buffers. These are currently limited to POD arguments
                        elif "cbuffer" in entrypoint_line:
                            entrypoint_line = entrypoint_line.replace("__HLSL_REGISTER", ":register(b{num_cbvs})".format(num_cbvs=num_cbvs))
                            num_cbvs += 1
                        # Handle writeable resources. These are bound as UAVs
                        elif "RW" in entrypoint_line:
                            entrypoint_line = entrypoint_line.replace("__HLSL_REGISTER", ":register(u{num_uavs})".format(num_uavs=num_uavs))
                            num_uavs += 1
                        # Handle read-only resources. These are bound as SRVs
                        elif "Texture" in entrypoint_line or "Buffer" in entrypoint_line:
                            entrypoint_line = entrypoint_line.replace("__HLSL_REGISTER", ":register(t{num_srvs})".format(num_srvs=num_srvs))
                            num_srvs += 1
                        elif "shared" in entrypoint_line:
                            entrypoint_line = entrypoint_line.replace("__HLSL_REGISTER", "")
                            import pdb; pdb.set_trace()
                        else:
                            import pdb; pdb.set_trace()
                    # This is a static shared memory case. No need to register but add it to the list of shared decls
                    elif "groupshared" in entrypoint_line:
                        current_entry["shared_decls"].append(entrypoint_line + ";\n")
                        continue
                    # We have hit the actual main function. We have to append the Threadblock and possibly root signature data before it
                    elif "void main" in entrypoint_line:
                        # Create and add the Root Signature containing the samplers
                        current_entry['raw_hlsl'].append("[RootSignature(\"")
                        if num_cbvs > 0:
                            current_entry['raw_hlsl'].append(f"DescriptorTable(CBV(b0,numDescriptors={num_cbvs}))".format(num_cbvs=num_cbvs))
                        if num_uavs > 0:
                            current_entry['raw_hlsl'].append(f",DescriptorTable(UAV(u0,numDescriptors={num_uavs}))".format(num_uavs=num_uavs))
                        if num_srvs > 0:
                            current_entry['raw_hlsl'].append(f",DescriptorTable(SRV(t0,numDescriptors={num_srvs}))".format(num_srvs=num_srvs))
                        if sampler_string:
                            current_entry['raw_hlsl'].append(sampler_string)
                        current_entry['raw_hlsl'].append("\")]\n")
                        # Threadblock data - We defer declaration of the actual blocksizes to handle certain edge cases in PFC kernels
                        current_entry['raw_hlsl'].append(f"[numthreads(__HLSL_BLOCK_SIZE_X, __HLSL_BLOCK_SIZE_Y, __HLSL_BLOCK_SIZE_Z)]\n")
                        num_cbvs = num_uavs = num_srvs = 0
                    # Handle last element of the entrypoint, we cannot have a semicolon at the end declaring main (and its definition)
                    if entrypoint_line != line_data.rsplit(';', 1)[1]:
                        entrypoint_line += ';' # Add the semicolon back
                    # Append to the raw hlsl
                    current_entry["raw_hlsl"].append(entrypoint_line + "\n")
                # We are officially in an entrypoint, so we can turn on the brace monitoring
                is_brace_monitoring_on = True
            else:
                # Generic line, add it to the raw hlsl
                current_entry["raw_hlsl"].append(line_data)
            if is_brace_monitoring_on:
                # Update number of braces
                num_braces += num_braces_inline
                # Only check if we just ended a scope
                if line_data.strip(";\n ").endswith("}") and num_braces <= 0:
                    # Turn off brace monitoring and switch back to common code g
                    is_brace_monitoring_on = False
                    current_entry = data_dict[common_name]
                    num_braces = 0

    hlsl_data = ''
    if data_dict[in_entrypoint]["shared_decls"]:
        # Write the shared decls first as globals. Other code comes later
        # Ensure we replace the unused token for the shared_decls you already have
        hlsl_data += '\n'.join(data_dict[in_entrypoint]["shared_decls"])
        for line_data in data_dict[common_name]['raw_hlsl']:
            pos = line_data.find("_shared_param_unused")
            if pos != -1:
                # Extract the name of the shared memory parameter
                smem_param = line_data[:pos].rsplit(' ', 1)[1]
                for decl in data_dict[in_entrypoint]["shared_decls"]:
                    # Remove unused if not already declared by globals
                    if not re.search(r'\b' + smem_param + r'\b', decl):
                        line_data = line_data.replace("_shared_param_unused", "")
                        break
            hlsl_data += line_data + '\n'
    hlsl_data += ''.join(data_dict[common_name]["raw_hlsl"])
    hlsl_data += ''.join(data_dict[in_entrypoint]["raw_hlsl"])
    return hlsl_data

def postprocessHLSL(args):
    if not os.path.exists(args.input_file):
        print('Input file missing! Exiting')
        sys.exit()

    if not os.path.exists(args.output_dir):
        print('Output directory missing - Creating one')
        os.makedirs(args.output_dir)
        if not os.path.exists(args.output_dir):
            print('Cannot create output directory! Exiting')
            sys.exit()
    output_file = os.path.join(args.output_dir, args.entrypoint + '.hlsl')
    hlsl_defines = "#define __HLSL_BLOCK_SIZE_X {blocksize[0]}\n#define __HLSL_BLOCK_SIZE_Y {blocksize[1]}\n#define __HLSL_BLOCK_SIZE_Z {blocksize[2]}\n#define __HLSL_SHARED_MEMORY_SIZE {shared_elements}\n".format(blocksize=args.blocksize,shared_elements=args.default_shared_elements)
    processedHLSL = hlsl_defines + processHLSL(args.input_file, args.entrypoint)
    with open(output_file, "w") as hlsl_file:
        hlsl_file.write(processedHLSL)

def main(argv):
    parser = argparse.ArgumentParser(description='Process intermediate files to generate HLSL')
    parser.add_argument('-i', '--input-file', help='path to clang intermediate file')
    parser.add_argument('-o', '--output-dir', help='output directory')
    parser.add_argument('-e', '--entrypoint', help='entry point to compile')
    parser.add_argument('--blocksize', help='blocksize for kernel launch', action='append', default=[16, 16, 1])
    parser.add_argument('--default-shared-elements', help='number of elements for dynamic shared memory', default=1024)

    args = parser.parse_args(argv[1:])
    postprocessHLSL(args)

if __name__ == '__main__':
    main(sys.argv)