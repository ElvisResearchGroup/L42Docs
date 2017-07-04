
// Test data for JSTree
var data = [
   'Simple root node',
   {
     'text' : "Root node 2",
     'icon' : "jstree-file", // string for custom
     'state' : {
       'opened' : true,
       'selected' : true
     },
     'children' : [
       { 'text' : 'Child 1' },
       'Child 2'
     ]
  }
];



// Convert a L42 string into a L42 Json object, then into a JSTree Json object.
var convertToTreeJson = function(string){

	// Turn string into json
	var parseJson = getJsonFromString(string);
	console.log("parsed " + parseJson);

	// Turn it into json that's readable by the tree
	var convertedJson = [convertRec(parseJson)];
	console.log("convertedJson " + convertedJson);


	return convertedJson;
}

// Recursively build a Json object.
// Should be given a root object.
// Given Type is:
    /*
        Object:
        name: string
        kind: string
        mdf: string
        doc: string
        content: [Objects]
    */

    // Turn it into the following
    /*
        Object:
        text : string
        icon : string
        doc : string
        children : []
    */
var convertRec = function(json){

    // Create a node for the tree
    // If we have children. Recursively make them
	var parent =  {
        'text' : getName(json), // Use l42JsonConverter.js's getName method to get the name
        'icon' : getIcon(json), // Directory to an image
        'doc' : json.doc,       // String with all the information
        'children' : getChildren(json) // Array of Json objects as children
    };

    return parent;
}

// Get the idcon 
var getIcon = function(json){
    if( json.kind.endsWith("Class")){
        return "icons/icon_class.png";
    }
    else{
        return "icons/icon_method.png";
    }
}

// Create the children for the tree
var getChildren = function(json){
    var children = [];
    for (var i = 0; i < json.content.length; i++) {
        var child = convertRec(json.content[i]);
        children.push(child);
    }
    return children;
}




// Format the documentation nicely
var formatDocumentation = function(string){
    /*var outterDiv = document.createElement('div'); 

    var startIndex = 0;
    var newLineIndex = string.indexOf("\n");
    while( newLineIndex != -1 ){
        console.log("Index " + startIndex + "," + newLineIndex);

        // Save text block
        var innerDiv = document.createElement('p');
        innerDiv.innerHTML = string.substring(startIndex,newLineIndex);
        outterDiv.appendChild(innerDiv);

        // Start next block
        startIndex = newLineIndex+1;

        // Find next new line
        newLineIndex = string.indexOf(startIndex,"\n");
    }
    console.log("Index " + newLineIndex);*/

    // Removed the above code as it just broke everything further.
    var outterDiv = document.createElement('span'); 
    outterDiv.innerHTML = string;
    return outterDiv;
}

//
// TEMP CODE
// Used to test the documentation section
//
window.onload = function() {
    var fileInput = document.getElementById('fileInput');
    var fileDisplayArea = document.getElementsByClassName('documentation')[0];

    fileInput.addEventListener('change', function(e) {
        var file = fileInput.files[0];
        var textType = /text.*/;

        if (file.type.match(textType)) {
            var reader = new FileReader();

            reader.onload = function(e) {
                while ( fileDisplayArea.firstChild ) fileDisplayArea.removeChild( fileDisplayArea.firstChild );
                fileDisplayArea.appendChild(formatDocumentation(reader.result));
            }

            reader.readAsText(file);    
        } else {
            fileDisplayArea.innerText = "File not supported!"
        }
    });
}