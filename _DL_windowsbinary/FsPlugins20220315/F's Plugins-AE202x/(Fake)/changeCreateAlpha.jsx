(function(me){
var winObj = ( me instanceof Panel) ? me : new Window("palette", "changeCreateAlpa", [ 0,  0,  470,  220]  ,{resizeable:true, maximizeButton:true, minimizeButton:true});
	//-------------------------------------------------------------------------
    var ctrlTbl = [];
    var yy = 10;
    var cap1 = winObj.add("statictext", [  20,    yy,   20 + 430,    yy +  24], "全てのUnmultをcreateAlphaに変える" );
    ctrlTbl.push(cap1);
    yy += 30;
	var btnExec = winObj.add("button", [  20,    yy,   20 + 430,    yy +  24], "実行" );
    ctrlTbl.push(btnExec);
    yy += 40;
    //alert(yy);
 //--------------------------------------------------------------------
 var ACcnt = 0;
 var removeUnmult = function(cmp)
 {
     if ( (cmp instanceof CompItem)==false) return;
     if (cmp.numLayers<=0) return;
     for (var i=1; i<=cmp.numLayers;i++)
     {
        var lyr = cmp.layer(i);
        var eg = lyr.property("ADBE Effect Parade");
	    var eNum = eg.numProperties;
        if (eNum<=0) continue;
        for(var j=eNum; j>=1;j--)
        {
            var mn = eg(j).matchName;
            if ( (mn=="KNSW Unmult")||(mn=="RG unmult") )
            {
                var ca = eg(j).parentProperty.addProperty("F's CreateAlpha");
                ca.moveTo(eg(j).propertyIndex+1);
                eg(j).remove();
                ACcnt++;
            }

        }
     }
 }
 //--------------------------------------------------------------------
 var exec = function()
 {
     if (app.project.numItems<=0)
     {
         alert("no item");
         return;
     }
     var sel =[];
     for (var i=1; i<=app.project.numItems;i++)
     {
         if (app.project.items[i] instanceof CompItem)
         {
             if (app.project.items[i].numLayers>0){
                sel.push(app.project.items[i]);
             }
         }
     }
     if (sel.length<=0){
         alert("no target");
         return;
     }
     app.beginUndoGroup("changeCreateAlpa");
     ACcnt = 0;
     for ( var i=0; i<sel.length;i++)
     {
         removeUnmult(sel[i]);
     }
     app.endUndoGroup();
     if (ACcnt==0)
     {
         alert("変更なし");
     }else{
         alert(ACcnt+"箇所変更しました");
     }
 }
 btnExec.onClick = exec;
 //--------------------------------------------------------------------
    var resizeFunc  = function()
    {
        var b = winObj.bounds;
        var w = b[2]-b[0];

       for (var i=0; i<ctrlTbl.length;i++)
       {
           var bb = ctrlTbl[i].bounds;
           bb[2] = bb[0] + w -40;
           ctrlTbl[i].bounds = bb;
       }

    }
    resizeFunc();
    winObj.onResize = resizeFunc;
 //--------------------------------------------------------------------
	if ( ( me instanceof Panel) == false){
		winObj.center();
		winObj.show();
	}
})(this);