#include "Star.h"
#include <thread>
#include <vector>
#include <mutex>
#include <memory>

template <typename T>
inline T* GetPixelPtr(PF_EffectWorld* world, A_long x, A_long y) {
	if (!world || !world->data) return NULL;
	if (x < 0 || x >= world->width || y < 0 || y >= world->height) return NULL;
	return reinterpret_cast<T*>((char*)world->data + (y * world->rowbytes) + (x * sizeof(T)));
}

// スレッド対応版
template <typename T>
static PF_Err DrawLineT_Threaded(
	PF_EffectWorld* maskP,
	PF_EffectWorld* outputP,
	ParamInfo* infoP,
	int line_idx,
	A_long start_y,
	A_long end_y,
	std::shared_ptr<std::vector<std::mutex>> pixel_mutexes  // shared_ptrに変更
) {
	PF_Err err = PF_Err_NONE;
	PF_FpLong max_chan = GetMaxChannel<T>();
	
	PF_FpLong line_len = infoP->overall_length * infoP->lines[line_idx].len_per;
	PF_FpLong line_int = infoP->overall_intensity * infoP->lines[line_idx].int_per;
	if (line_len <= 0 || line_int <= 0) return err;
	
	PF_FpLong angle = infoP->overall_angle + infoP->lines[line_idx].angle_offset;
	PF_FpLong dx = sin(angle);
	PF_FpLong dy = -cos(angle);
	int i_len = (int)line_len;

	if (i_len < 0 || i_len > 10000) return PF_Err_OUT_OF_MEMORY;
	
	std::vector<PF_FpLong> falloff_lut;
	try {
		falloff_lut.resize(i_len + 1);
	} catch (...) {
		return PF_Err_OUT_OF_MEMORY;
	}
	
	for (int i = 0; i <= i_len; i++) {
		PF_FpLong dist_ratio = (PF_FpLong)i / line_len;
		falloff_lut[i] = pow(1.0 - dist_ratio, 5.0) * line_int;
	}
	
	PF_FpLong inv_max = 1.0 / max_chan;

	for (A_long y = start_y; y < end_y; y++) {
		for (A_long x = 0; x < maskP->width; x++) {
			T* srcP = GetPixelPtr<T>(maskP, x, y);
			if (!srcP || srcP->alpha == 0) continue;

			for (int i = -i_len; i <= i_len; i++) {
				if (i == 0) continue;

				PF_FpLong fx = (PF_FpLong)x + (dx * i);
				PF_FpLong fy = (PF_FpLong)y + (dy * i);

				A_long x1 = (A_long)floor(fx);
				A_long y1 = (A_long)floor(fy);
				A_long x2 = x1 + 1;
				A_long y2 = y1 + 1;

				PF_FpLong wx2 = fx - x1;
				PF_FpLong wy2 = fy - y1;
				PF_FpLong wx1 = 1.0 - wx2;
				PF_FpLong wy1 = 1.0 - wy2;

				int abs_i = (i < 0) ? -i : i;
				if (abs_i >= (int)falloff_lut.size()) continue;
				PF_FpLong amt = falloff_lut[abs_i];

				struct { A_long x; A_long y; PF_FpLong w; } samples[4] = {
					{x1, y1, wx1 * wy1},
					{x2, y1, wx2 * wy1},
					{x1, y2, wx1 * wy2},
					{x2, y2, wx2 * wy2}
				};

				for (int s = 0; s < 4; s++) {
					if (samples[s].w <= 0.0001) continue;
					
					if (samples[s].x < 0 || samples[s].x >= outputP->width ||
					    samples[s].y < 0 || samples[s].y >= outputP->height) {
						continue;
					}
					
					T* dstP = GetPixelPtr<T>(outputP, samples[s].x, samples[s].y);
					if (!dstP) continue;
					
					// マルチスレッド時のみロック
					if (pixel_mutexes && pixel_mutexes->size() > 0) {
						A_long pixel_index = samples[s].y * outputP->width + samples[s].x;
						size_t mutex_index = pixel_index % pixel_mutexes->size();
						std::lock_guard<std::mutex> lock((*pixel_mutexes)[mutex_index]);
						
						PF_FpLong total_amt = amt * samples[s].w;
						PF_FpLong r2 = srcP->red * total_amt;
						PF_FpLong g2 = srcP->green * total_amt;
						PF_FpLong b2 = srcP->blue * total_amt;
						PF_FpLong a2 = srcP->alpha * total_amt;
						
						PF_FpLong r = (PF_FpLong)dstP->red + r2 - (PF_FpLong)dstP->red * r2 * inv_max;
						PF_FpLong g = (PF_FpLong)dstP->green + g2 - (PF_FpLong)dstP->green * g2 * inv_max;
						PF_FpLong b = (PF_FpLong)dstP->blue + b2 - (PF_FpLong)dstP->blue * b2 * inv_max;
						PF_FpLong a = (PF_FpLong)dstP->alpha + a2 - (PF_FpLong)dstP->alpha * a2 * inv_max;

						dstP->red = (typename PixelTraits<T>::channel_type)AE_CLAMP(r, 0, max_chan);
						dstP->green = (typename PixelTraits<T>::channel_type)AE_CLAMP(g, 0, max_chan);
						dstP->blue = (typename PixelTraits<T>::channel_type)AE_CLAMP(b, 0, max_chan);
						dstP->alpha = (typename PixelTraits<T>::channel_type)AE_CLAMP(a, 0, max_chan);
					} else {
						// シングルスレッド時はロックなし
						PF_FpLong total_amt = amt * samples[s].w;
						PF_FpLong r2 = srcP->red * total_amt;
						PF_FpLong g2 = srcP->green * total_amt;
						PF_FpLong b2 = srcP->blue * total_amt;
						PF_FpLong a2 = srcP->alpha * total_amt;
						
						PF_FpLong r = (PF_FpLong)dstP->red + r2 - (PF_FpLong)dstP->red * r2 * inv_max;
						PF_FpLong g = (PF_FpLong)dstP->green + g2 - (PF_FpLong)dstP->green * g2 * inv_max;
						PF_FpLong b = (PF_FpLong)dstP->blue + b2 - (PF_FpLong)dstP->blue * b2 * inv_max;
						PF_FpLong a = (PF_FpLong)dstP->alpha + a2 - (PF_FpLong)dstP->alpha * a2 * inv_max;

						dstP->red = (typename PixelTraits<T>::channel_type)AE_CLAMP(r, 0, max_chan);
						dstP->green = (typename PixelTraits<T>::channel_type)AE_CLAMP(g, 0, max_chan);
						dstP->blue = (typename PixelTraits<T>::channel_type)AE_CLAMP(b, 0, max_chan);
						dstP->alpha = (typename PixelTraits<T>::channel_type)AE_CLAMP(a, 0, max_chan);
					}
				}
			}
		}
	}
	return err;
}

template <typename T>
static PF_Err DrawLineT(
	PF_EffectWorld* maskP,
	PF_EffectWorld* outputP,
	ParamInfo* infoP,
	int line_idx
) {
	if (!maskP || !outputP || !infoP) return PF_Err_BAD_CALLBACK_PARAM;
	if (!maskP->data || !outputP->data) return PF_Err_BAD_CALLBACK_PARAM;
	
	PF_Err err = PF_Err_NONE;
	
	unsigned int num_threads = std::thread::hardware_concurrency();
	if (num_threads == 0) num_threads = 4;
	num_threads = MIN(num_threads, 8);  // 最大8スレッドに制限
	
	// シングルスレッド処理
	if (maskP->height < (A_long)num_threads * 16) {
		return DrawLineT_Threaded<T>(maskP, outputP, infoP, line_idx, 0, maskP->height, nullptr);
	}
	
	// マルチスレッド処理
	auto pixel_mutexes = std::make_shared<std::vector<std::mutex>>(2048);  // shared_ptrで管理
	std::vector<std::thread> threads;
	A_long rows_per_thread = maskP->height / num_threads;
	
	for (unsigned int t = 0; t < num_threads; t++) {
		A_long start_y = t * rows_per_thread;
		A_long end_y = (t == num_threads - 1) ? maskP->height : (t + 1) * rows_per_thread;
		
		threads.emplace_back([maskP, outputP, infoP, line_idx, start_y, end_y, pixel_mutexes]() {
			DrawLineT_Threaded<T>(maskP, outputP, infoP, line_idx, start_y, end_y, pixel_mutexes);
		});
	}
	
	for (auto& thread : threads) {
		if (thread.joinable()) {
			thread.join();
		}
	}
	
	return err;
}

PF_Err DrawStar(NF_AE* ae, PF_EffectWorld* mask_world, ParamInfo* infoP) 
{
	if (!ae || !mask_world || !infoP) return PF_Err_BAD_CALLBACK_PARAM;
	if (!ae->output || !ae->output->data) return PF_Err_BAD_CALLBACK_PARAM;
	
	PF_Err err = PF_Err_NONE;
	PF_PixelFormat pixel_format = ae->pixelFormat();
	
	for (int i = 0; i < 4; i++) {
		switch (pixel_format) {
		case PF_PixelFormat_ARGB128: 
			err = DrawLineT<PF_PixelFloat>(mask_world, ae->output, infoP, i); 
			break;
		case PF_PixelFormat_ARGB64:  
			err = DrawLineT<PF_Pixel16>(mask_world, ae->output, infoP, i); 
			break;
		case PF_PixelFormat_ARGB32:  
			err = DrawLineT<PF_Pixel8>(mask_world, ae->output, infoP, i); 
			break;
		default:
			return PF_Err_UNRECOGNIZED_PARAM_TYPE;
		}
		
		if (err) return err;
	}
	
	return err;
}