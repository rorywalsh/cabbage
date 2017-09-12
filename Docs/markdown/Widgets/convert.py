import markdown2
import codecs
from os.path import basename
import os, sys


mkin = open(sys.argv[1])
source = mkin.read()
gen_html = markdown2.markdown(source, extras = ['toc(title=Table of Contents)', 'codehilite','meta','include','fenced_code'])
# gen_html = md.convert(mkin.read())

# doc_title = md_meta.get('title')[0] # [0] -> converts one element list to string
doc_title ="My Title"

fullHtmlOutput = [ """
<!doctype html>
<html>
<head>
<meta charset="utf-8">
<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Sliding Push Navigation Menu Demo</title>
<link rel="stylesheet" href="http://netdna.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">
<link href='http://fonts.googleapis.com/css?family=Roboto+Condensed' rel='stylesheet' type='text/css'>
<link href='../Styles/menu.css' rel='stylesheet' type='text/css'>
<link href='../../Styles/menu.css' rel='stylesheet' type='text/css'>
<link href='./Styles/menu.css' rel='stylesheet' type='text/css'>

</head>


<body>
<input type="checkbox" id="menu-toggle" />
<label for="menu-toggle" class="menu-icon"><i class="fa fa-bars"></i></label>
<div class="content-container">
  <div class="css-script-ads" style="margin:30px auto;">
    <script type="text/javascript"><!--
google_ad_client = "ca-pub-2783044520727903";
/* CSSScript Demo Page */
google_ad_slot = "3025259193";
google_ad_width = 728;
google_ad_height = 90;
//-->
</script>
</div>
<div class="content">

""" % { 'title' : doc_title }
]

fullHtmlOutput.append( ''.join(gen_html) )

fullHtmlOutput.append( """

  </div>
</div>
<div class="slideout-sidebar">
  	<ul><a href="readme.html">Cabbage User Manual</a>
  	<li></i> <a href="readme.html">Cabbage User Manual</a></li>
  	<li></i> <a href="csound.html">Beginners (Csound)</a></li>
		<ul>
		<li></i> <a href="file_structure_and_syntax.html">File Structure and Synthax</a></li>
		<li></i> <a href="keywords.html">Keywords</a></li>
		<li></i> <a href="constants_and_variables.html">Constants and Variables</a></li>
		<li></i> <a href="opcodes.html">Opcodes</a></li>
		<li></i> <a href="operators_and_comments.html">Operators and Comments</a></li>
		</ul>
	<li></i> <a href="introduction.html">Using Cabbage</a></li>
		<ul>
		<li></i> <a href="using_cabbage.html">Introduction</a></li>
		<li></i> <a href="cabbage_syntax.html">Cabbage Syntax</a></li>
		<li></i> <a href="beginner_synth.html">A simple Synthesiser</a></li>
		<li></i> <a href="first_effect.html">A simple Effect</a></li>
		<li></i> <a href="audio_graph.html">The Cabbage patcher</a></li>
		<li></i> <a href="exporting.html">Exporting plugins</a></li>
		<li></i> <a href="cabbage_in_host.html">Using plugins in 3rd Party software</a></li>
		<li></i> <a href="distributing.html">Distributing instruments</a></li>
		
    
    <li></i> About Us</li>
    <li></i> Blog</li>
    <li></i> Contact</li>
  </ul>
</div>
</body>
</html>
""")

filename = os.path.splitext(sys.argv[1])[0]+".html";
output_file = codecs.open(filename, "w", 
                          encoding="utf-8", 
                          errors="xmlcharrefreplace")
output_file.write('\n'.join(fullHtmlOutput))
output_file.close()

# end