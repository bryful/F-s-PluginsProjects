var  FsAbout = function()
{
	var strName = "%s";
	var strVersion = "version %d.%d";
	var strDis = "%s";
	var strMyName = "https://github.com/bryful : bryful@gmail.com ";
    var nanae = "Nanae Furuhashi - My daughter, May her soul rest in peace．";
	var winObj = new Window("dialog", "NF's Plugins", [ 0,  0,  480, 180] );

	var edFsName = winObj.add("edittext", [  30,   10,   30+ 440,   10+  20], strName, { readonly:true, borderless:true });
	var edFsVersion = winObj.add("edittext", [  30,   40,   70+ 400,   40+ 20], strVersion, { readonly:true, borderless:true });
	var edFsDis = winObj.add("edittext", [  30,   70,   30+ 440,   70+  20], strDis, { readonly:true, borderless:true });
	var edMyName = winObj.add("edittext", [  30,  100,   30+ 440,  100+  20], strMyName, { readonly:true, borderless:true });
    var stNana = winObj.add("statictext", [  30,  120,   30+ 440,  120+  20], nanae, { readonly:true, borderless:true });
	var btnOK = winObj.add("button", [ 360,  140,  360+ 100,  140+  24], "OK" , { name:"ok" });
	this.show = function()
	{
		winObj.center();
		return winObj.show();
	}
}
var dlg = new FsAbout;
dlg.show();


