function showColorNameDialog(count, namesArray) {
    var win = new Window("dialog", "Edit Color Names");
    win.orientation = "column";
    win.alignChildren = ["fill", "top"];
    win.spacing = 10;
    win.margins = 16;

    // スクロール可能なエリア（項目が多い場合を想定）
    var scrollGroup = win.add("group");
    scrollGroup.orientation = "column";
    scrollGroup.alignChildren = ["left", "top"];
    scrollGroup.maximumSize.height = 500; // 32個でも画面からはみ出さないよう制限

    var panel = scrollGroup.add("panel", undefined, "Active Colors: " + count);
    panel.alignChildren = ["left", "top"];

    var edits = [];
    for (var i = 0; i < count; i++) {
        var row = panel.add("group");
        row.orientation = "horizontal";

        var lbl = row.add("statictext", undefined, (i + 1) + ":");
        lbl.characters = 3;

        // 既存の名前があれば入れ、なければ作成
        var nameVal = (i < namesArray.length) ? namesArray[i] : "color" + (i + 1);
        var et = row.add("edittext", undefined, nameVal);
        et.characters = 25;
        edits.push(et);
    }

    // ボタンエリア
    var btnGroup = win.add("group");
    btnGroup.alignment = "right";
    var okBtn = btnGroup.add("button", undefined, "OK", {
        name: "ok"
    });
    var cancelBtn = btnGroup.add("button", undefined, "Cancel", {
        name: "cancel"
    });

    if (win.show() == 1) {
        var results = [];
        for (var j = 0; j < count; j++) {
            results.push(edits[j].text);
        }
        return results.join("|"); // C++側でパースしやすいようパイプで連結
    }
    return null;
}
var testCount = 8;
var testNames = ["Red", "Green", "Blue", "Yellow", "Cyan", "Magenta", "White", "Black"];
var res = showColorNameDialog(testCount, testNames);
if (res !== null) {
    alert("Returned to C++:\n" + res);
}