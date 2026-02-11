function showColorNameDialog(count, namesArray) {
    var win = new Window("dialog", "Edit Color Names");
    win.orientation = "column";
    win.alignChildren = ["fill", "top"];
    win.spacing = 10;
    win.margins = 16;

    var scrollGroup = win.add("group");
    scrollGroup.orientation = "column";
    scrollGroup.alignChildren = ["left", "top"];
    scrollGroup.maximumSize.height = 500;

    var panel = scrollGroup.add("panel", undefined, "Active Colors: " + count);
    panel.alignChildren = ["left", "top"];

    var edits = [];
    for (var i = 0; i < count; i++) {
        var row = panel.add("group");
        row.orientation = "horizontal";
        var lbl = row.add("statictext", undefined, (i + 1) + ":");
        lbl.characters = 3;
        var nameVal = (i < namesArray.length && namesArray[i] !== "") ? namesArray[i] : "color" + (i + 1);
        var et = row.add("edittext", undefined, nameVal);
        et.characters = 25;
        edits.push(et);
    }

    var btnGroup = win.add("group");
    btnGroup.alignment = "right";
    win.add("button", undefined, "OK", {
        name: "ok"
    });
    win.add("button", undefined, "Cancel", {
        name: "cancel"
    });

    if (win.show() == 1) {
        var results = [];
        for (var j = 0; j < count; j++) {
            var t = edits[j].text;
            if (t.replace(/\s/g, "") === "") {
                t = "color" + (j + 1);
            }
            results.push(t);
        }
        return results.join("|");
    }
    return null;
}

var testCount = 8;
var testNames = ["Red", "Green", "Blue", "Yellow", "Cyan", "Magenta", "White", "Black"];
var res = showColorNameDialog(testCount, testNames);
if (res !== null) {
    alert("Returned to C++:\n" + res);
}