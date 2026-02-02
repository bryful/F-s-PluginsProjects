(function(me){
	var strName = "%s";
	var strVersion = "ver %d.%d [%s]";
	var strDis = "%s";
	var strMyName = "https://github.com/bryful";
    //var nanae = "Nanae Furuhashi - My beloved daughter. May she rest in peace.";
	var winObj = new Window("dialog", "NF's Plugins", [ 0,  0,  480, 180] );
	var edFsName = winObj.add("edittext", [  30,   12,   30+ 440,   12+  20], strName, { readonly:true, borderless:true });
	var edFsVersion = winObj.add("edittext", [  30,   42,   30+ 440,   42+ 20], strVersion, { readonly:true, borderless:true });
	var edFsDis = winObj.add("edittext", [  30,   72,   30+ 440,   72+  20], strDis, { readonly:true, borderless:true });
	var edMyName = winObj.add("edittext", [  30,  102,   30+ 440,  102+  20], strMyName, { readonly:true, borderless:true });
    //var stNana = winObj.add("statictext", [  180,  163,   180+ 440,  163+  20], nanae, { readonly:true, borderless:true });
	var btnOK = winObj.add("button", [ 360,  140,  360+ 100,  140+  24], "OK" , { name:"ok" });
	winObj.center();
	winObj.show();
})(this);
