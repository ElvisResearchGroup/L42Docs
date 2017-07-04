// Testable L42 Json String
var testL42JsonString = 
 '{"kind":"OpenClass", "mdf":"Immutable", "name":"Foo", "doc":"foo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very longfoo doc\\n many lines\\n some line could be very very long", "content":[\n'
+' { "kind":"AbstractMethod", "mdf":"Class", "name":"+(that)", "doc":"plus docs", "content":[]},\n'
+' { "kind":"ImplementedMethod ", "mdf":"Lent", "name":"foo(bar,beer)", "doc":"foobarbeer\\n\\n docs", "content":[]},\n'
+' { "kind":"Interface", "mdf":"Immutable", "name":"Bar", "doc":"bar doc", "content":[ \n'
+'       { "kind":"AbstractMethod", "mdf":"Capsule", "name":"+(that)", "doc":"plus docs", "content":[]} \n'
+'       ]}\n'
+'   ]}';

/*
    '{"kind":"OpenClass", "mdf":"Immutable", "name":"ShowAllKindExpressions", "doc":"LONG", "content":['+
    '{"kind":"OpenClass", "mdf":"Immutable", "name":"AllKinds", "doc":"Documentation about OpenClass", "content":['+
    '{"kind":"Interface","mdf":"Immutable", "name":"Interface", "doc":"Documentation about Interface", "content":[]},'+
    '{"kind":"ClosedClass","mdf":"Immutable", "name":"ClosedClass", "doc":"Documentation about ClosedClass", "content":[]},'+
    '{"kind":"OpenClass","mdf":"Immutable", "name":"OpenClass", "doc":"Documentation about OpenClass", "content":[]},'+
    '{"kind":"Template","mdf":"Immutable", "name":"Template", "doc":"Documentation about Template", "content":[]},'+
    '{"kind":"FreeTemplate","mdf":"Immutable", "name":"FreeTemplate", "doc":"Documentation about FreeTemplate", "content":[]},'+
    '{"kind":"InterfaceImplementedMethod","mdf":"Immutable", "name":"InterfaceImplementedMethod", "doc":"Documentation about InterfaceImplementedMethod", "content":[]},'+
    '{"kind":"ImplementedMethod","mdf":"Immutable", "name":"ImplementedMethod", "doc":"Documentation about ImplementedMethod", "content":[]},'+
    '{"kind":"AbstractMethod","mdf":"Immutable", "name":"AbstractMethod", "doc":"Documentation about AbstractMethod", "content":[]}'+
    ']},'+
    '{"kind":"OpenClass","mdf":"Immutable","name":"AllMDFs","doc":"bar doc","content":['+
    '{"kind":"AbstractMethod","mdf":"Immutable", "name":"Immutable", "doc":"Documentation about Immutable", "content":[]},'+
    '{"kind":"AbstractMethod","mdf":"Mutable", "name":"Mutable", "doc":"Documentation about Mutable", "content":[]},'+
    '{"kind":"AbstractMethod","mdf":"Lent", "name":"Lent", "doc":"Documentation about Lent", "content":[]},'+
    '{"kind":"AbstractMethod","mdf":"Read", "name":"Read", "doc":"Documentation about Read", "content":[]},'+
    '{"kind":"AbstractMethod","mdf":"Class", "name":"Class", "doc":"Documentation about Class", "content":[]},'+
    '{"kind":"AbstractMethod","mdf":"Capsule", "name":"Capsule", "doc":"Documentation about Capsule", "content":[]}'+
    ']}'+
    ']}';
*/

/*
-Expected Json Format-
Object:
	name: string
	kind: string
	mdf: string
	doc: string
	content: [Objects]
*/

// Gets the name of the tree and styles it accordingly
var getName = function(object){
  // Get name from the object
  var name = String(object.name);

  // Kind Prefix
  var prefix = getKindPrefix(object.kind);
  name = prefix + name;

  // Mdf Prefix
  var mdf = getMDFPrefix(object.mdf);
  name = mdf + name;

  // Return the name with styling
  return String(name);
}

// Get the letter we want according to the kind. And style it
// Throw an error if it does not exist
var getKindPrefix = function(kind){
  var letter = '<span class="kind"/>';
  if( kind === "Interface" )                   letter = "I";
  if( kind === "OpenClass" )                   letter = "C";
  if( kind === "ClosedClass" )                 letter = "C"; // Red C
  if( kind === "Template" )                    letter = "T";
  if( kind === "FreeTemplate" )                letter = "T";// Green
  if( kind === "InterfaceImplementedMethod" )  letter = "M"; // Blue M
  if( kind === "AbstractMethod" )              letter = "A"; // Blue A
  if( kind === "ImplementedMethod" )           letter = "M"; // Red M
  if( letter != "" ){
  	// This kind exists. Return the prefix with its styling
  	return '<span class="kind kind-'+kind+'">'+letter+'</span>'
  }
  else{

  	// This kind does NOT exist. Throw an error
  	throw ("Error passing Kind: Kind '" +kind + "' does not exist!");
  }

  
}

// Get the prefix from the MDF type
// If the mdf does not exist. Throw error!
var mdfArray = ["Mutable","Lent","Read","Class","Capsule"];
var getMDFPrefix = function(mdf){
  	if( mdf == "Immutable" ) {
	  return '<span class="mdf"></span>';  // Don't do anything for Immutable
	}
	else if( mdfArray.indexOf(mdf) != -1) {
	  	return '<span class="mdf mdf-'+mdf+'">@</span>'
	}
  	else{

  		// This MDF does NOT exist. Throw an error
  		throw ("Error passing Mdf: Mdf '" +mdf + "' does not exist!");
  	}
}


// Checks if the given json file is of L42 validity
var jsonIsValid = function(json, path){

	// Null check
	if( json == null ){
		throw "Error passing Json: Json is null!";
	}
	
	// Check name
	if( json.name == null ){
		throw "Error passing Json: Object " + path + " is missing 'name'!";	
	}

	// Check kind
	if( json.kind == null ){
		throw "Error passing Json: Object " + path + "->" + json.name + " is missing 'kind'!";	
	}

	// Check mdf
	if( json.mdf == null ){
		throw "Error passing Json: Object " + path + "->" + json.name + " is missing 'mdf'!";	
	}

	// Check doc
	if( json.doc == null ){
		throw "Error passing Json: Object " + path + "->" + json.name + " is missing 'doc'!";	
	}

	// Check content
	if( json.content == null ){
		throw "Error passing Json: Object " + path + "->" + json.name + " is missing 'content'!";	
	}

	// Check children with a path leading from root down to the child
	if( json.content.length > 0 ){
		for (var i = 0; i < json.content.length; i++) {
			jsonIsValid(json.content[i],path + "->" + json.name);
		}
	}

	// Passed all tests
	return true;
}

// Converts a string into a json object ad tests it
var getJsonFromString = function(string){
	var parsed = JSON.parse(String(string));

	// Check that it's not broken
	if( jsonIsValid(parsed, "root")){
		return parsed;
	}

	// Something is wrong with the file
	return null;
}