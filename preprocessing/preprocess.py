#############################################################
# Preprocessing of DBPedia file with Wikipedia long abstracts
# for the Serelex input. The script (1) reads the definitions;
# (2) clens them from the RDF markup; (3) removes articles with 
# titles composed of several words, words with numbers, and 
# some other titles, leaveing only articles with single word
# titles; (4) lemmatize and POS tag text of articles with TreeTagger.
# The output file contains words with definitnions in the 
# Serelex format. Before running the script you should 
# install TreeTagger with English grammars and modify the 
# TREETAGGER_ROOT constant. 
############################################################

import sys
import os
import re
import treetaggerwrapper
import string

TREETAGGER_ROOT = '/home/sasha/work/icop/treetagger/'

def cleanup_dbpedia_abstract(dbpedia_file, output_file):
# Performs preprocessing of the DBPedia abstracts file
# and writes it to the output file in the format "target; definition"
# This cleanup is highly adopted to the experiments with the BLESS, SN, and RG 
# datasets of semantic relations, since all multi-word expression, personal names,
# abbreviations, etc. are striped out

    print "Cleaning up DBpedia definitions..."
  
    # Open the files
    dbpedia_file = open(dbpedia_file, "r")
    output_file = open(output_file,"w")
    
    # Read DBPedia definitions line by line
    def_count = 0
    def_total_count = 0
    
    for line in dbpedia_file:        
        # Skip the void lines
        if line.strip() == "": continue

       	# Extract the target and definition 
       	(original_target, definition)= tokenize_line(line)
       	(target, original_target) = clean_target(original_target)
       	def_total_count = def_total_count + 1
       	if target == "": continue 
       	definition = clean_definition(definition)
       	if definition == "": continue
       	
       	# Write the target and definition to the output file
       	output_file.write(target + ";" + original_target + ". " + definition + "\n\n")
       	def_count = def_count + 1
       	#print target       			
    
    print def_count, "definitions were written out of", def_total_count, " definitions in the input file."
            	
    # Save the changes
    dbpedia_file.close()    
    output_file.close()

   
def clean_target(target):
# Cleans the target. If the target does not fit the selection criteria (MWE, stop word, 
# the main meaning of the word, etc.) then "" is returned
# Returns (cleaned target, original target) tuple 
   
    original_target = target   
	# Fast check - non-single words are rejected
    not_single_word = re.search("[_\s,\.]", target, flags=re.IGNORECASE)
    if not_single_word: return ("", "")
    
    # Fast check - Remove targets with capital letters (exept the first one)
    capital_letters = re.search("^..*[A-Z].*", target)
    if capital_letters: return ("", "")
    
    # Long check - various criteria
    target = target.lower()    
    delete_target = (
                    re.search("%\d\d", target, flags=re.IGNORECASE) or
                    re.search("%[a-f][a-f]", target, flags=re.IGNORECASE) or
                    re.search("\d+", target, flags=re.IGNORECASE) or
                    re.search("%[a-f]\d", target, flags=re.IGNORECASE) or
                    re.search("&[a-z]+", target, flags=re.IGNORECASE) or
                    re.search(r"\\", target, flags=re.IGNORECASE) or
                    re.search("\"", target, flags=re.IGNORECASE) or
                    re.search(r"\\u....", target, flags=re.IGNORECASE) or
                    re.search(r"[|{}#@><~&+=*\/]", target, flags=re.IGNORECASE) or
                    re.search("&emsp", target, flags=re.IGNORECASE) or
                    re.search("&thinsp", target, flags=re.IGNORECASE) or
                    re.search("&emsp", target, flags=re.IGNORECASE) or
                    re.search("&ndash", target, flags=re.IGNORECASE) or 
                    re.search("&gt", target, flags=re.IGNORECASE) or 
                    re.search("&lt", target, flags=re.IGNORECASE))
    if delete_target: 
        return ("", "")
    
    # Make sure that the targets does not contain delimiters
    target = re.sub("[;\.]","", target)
    original_target = re.sub("[;\.]","", original_target)
        
    return (target, original_target)  
	
    
def clean_definition(definition):
# Cleans the text of the DBPedia definition. 
    definition = re.sub(";"," ", definition)
    definition = re.sub("@en .", "", definition)
    definition = re.sub(r"\\", "", definition)
    definition = re.sub("\"", "", definition)    
    definition = re.sub("%\d\d", "", definition)
    definition = re.sub("%[a-f][a-f]", " ", definition)
    definition = re.sub("\d+", " ", definition)
    definition = re.sub("%[a-f]\d", " ", definition)
    definition = re.sub("&[a-z]+", " ", definition)
    definition = re.sub(r"\\u....", " ", definition)
    definition = re.sub(r"[|{}#@><~&+=*\/]", " ", definition)
    definition = re.sub("&emsp", "", definition)
    definition = re.sub("&thinsp", "", definition)
    definition = re.sub("&emsp", "", definition)
    definition = re.sub("&gt", "", definition)
    definition = re.sub("&lt", "", definition)
    definition = re.sub("\s+", " ", definition)
    return definition.strip()


def tokenize_line(line):
# Returns a tuple (target, definition) from a DBPedia definition
# An example of the definition: <http://dbpedia.org/resource/Alonzo_Church> <http://www.w3.org/2000/01/rdf-schema#comment> \"Ontology is the philosop...
	target = ""
	definition = ""
	
	# Get the target from the definition
	match = re.search("<http://dbpedia.org/resource/([^>]+)> <http://www.w3.org/2000/01/rdf-schema#comment> (.*)", line)
	if match:
		target = match.group(1)
		definition = match.group(2)		
	else: 
		return ("error","error")
		
	return (target, definition)

def lemmatize(definitions_file, output_file):
# Creates a lemmatized version of the DBpedia definitions 
    print "Lemmatization of the DBpedia definitions..."
    # Initialize the tagger
    DEF_START_TAG = ";\t:\t;"
    tagger = treetaggerwrapper.TreeTagger(TAGLANG='en',TAGDIR=TREETAGGER_ROOT)
    
    # Open the files
    defs = open(definitions_file).read()
    output = open(output_file,"w")
    
    # POS tag the definitions file
    tags = tagger.TagText(defs)

    # Write the POS tagged text into a new file
    for index, tag in enumerate(tags):
        # Get the token (surface, pos, lemma)
        token = tag.split("\t")
        if len(token) != 3: 
            print token
            continue
        original_token = token[0] 
        token[0] = token[0].lower()
        token[2] = token[2].lower() 
                
        # Write the token to the output file
        if( (len(tags) > index + 1) and (tags[index + 1] == DEF_START_TAG) ):
            # Write the target unlemmatized
            output.write("\n\n")
            output.write(token[0])            
        elif(tags[index] == DEF_START_TAG):
            # Write the separator
            output.write(";")
        elif (tags[index - 1] == DEF_START_TAG):
            # Write the original target 
            output.write(original_token)
        else:
            # Write other tags in the 'surface#POS#lemma' format
            if re.search("<unknown>", tag, flags=re.IGNORECASE): 
                output.write(" " + token[0] + "#" + token[1] + "#" + token[0] + " ")
            else:
                output.write(" " + token[0] + "#" + token[1] + "#" + token[2] + " ")
    # Save the output file
    output.close()

    # Remove <unknown word lemmas>
    #:%s/\([a-z-0-9]\+\)#\([a-z]\+\)#/\1#\2#\1/gc

#####################################
# Entry point
#####################################

#Process parameters
PARAM_NUM = 2
if len(sys.argv) != PARAM_NUM + 1:
	print "Expected", PARAM_NUM, "parameters but was", str(len(sys.argv)-1)
	print "Usage:", sys.argv[0], "<dbpedia-abstracts-file> <serelex-definitions-file>"
	print "<dbpedia-abstracts-file>\tInput DBPedia file with Wikipedia abstracts"
	print "<serelex-definitions-file>\tOuput Serelex file with definitions derived from Wikipedia abstracts"	

	sys.exit()

# Read the command line arguments
dbpedia_file = sys.argv[1]
output_file = sys.argv[2]
tmp_file = output_file+".tmp"

cleanup_dbpedia_abstract(dbpedia_file, tmp_file)
lemmatize(tmp_file, output_file)
os.remove(tmp_file)

print "Script has finished successfully."



