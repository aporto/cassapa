<html>

<head>

</head>

<body lang=EN-US style='tab-interval:.5in'>

<div class=WordSection1>

<p class=MsoTitleCxSpFirst align=center style='text-align:center'><span
lang=PT-BR style='mso-ansi-language:PT-BR'>Cassapa<o:p></o:p></span></p>

<p class=MsoTitleCxSpLast align=center style='text-align:center'><span
lang=PT-BR style='mso-ansi-language:PT-BR'>Tutorial<o:p></o:p></span></p>

<h1 style='margin-left:19.8pt;text-indent:-19.8pt;mso-list:l3 level1 lfo2'><![if !supportLists]><span
lang=PT-BR style='mso-fareast-font-family:"Calibri Light";mso-fareast-theme-font:
major-latin;mso-bidi-font-family:"Calibri Light";mso-bidi-theme-font:major-latin;
mso-ansi-language:PT-BR'><span style='mso-list:Ignore'>1.<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp; </span></span></span><![endif]><span
lang=PT-BR style='mso-ansi-language:PT-BR'>Overview<o:p></o:p></span></h1>

<p class=MsoNormal><span class=SpellE>Cassapa</span> is system that uses
augmented-reality to help aiming at pool games. <span class=SpellE>Cassapa</span>
software is comprised by several files, but the most important are:</p>

<p class=MsoListParagraphCxSpFirst style='text-indent:-.25in;mso-list:l7 level1 lfo3'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]><b style='mso-bidi-font-weight:normal'>cassapa_server_win.exe</b>:
This is the main executable. It is responsible for processing the camera image
and detecting the pool game. </p>

<p class=MsoListParagraphCxSpMiddle style='text-indent:-.25in;mso-list:l7 level1 lfo3'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]><b style='mso-bidi-font-weight:normal'>Cassapa_projector.exe</b>:
This is the software responsible for the projector output. This functionality
was put on a separate application (Not inside <span class=SpellE>cassapa_server_win</span>)
in order to support a system with 2 separate computers (One for the camera, one
for the projector), but they can run on the same computer as well.</p>

<p class=MsoListParagraphCxSpLast style='text-indent:-.25in;mso-list:l7 level1 lfo3'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]><b style='mso-bidi-font-weight:normal'>cassapa_viewer.exe</b>:
This is the client software that controls and monitors the system. As the server
has a minimalist user interface, all control is done via this viewer.</p>

<h1><span class=GramE>2.Hardware</span> infrastructure</h1>

<p class=MsoNormal>In order to use <span class=SpellE>Cassapa</span> software,
you need to provide the following infrastructure:</p>

<p class=MsoNormal align=center style='text-align:center'><span
style='mso-no-proof:yes'><!--[if gte vml 1]><v:shapetype id="_x0000_t75"
 coordsize="21600,21600" o:spt="75" o:preferrelative="t" path="m@4@5l@4@11@9@11@9@5xe"
 filled="f" stroked="f">
 <v:stroke joinstyle="miter"/>
 <v:formulas>
  <v:f eqn="if lineDrawn pixelLineWidth 0"/>
  <v:f eqn="sum @0 1 0"/>
  <v:f eqn="sum 0 0 @1"/>
  <v:f eqn="prod @2 1 2"/>
  <v:f eqn="prod @3 21600 pixelWidth"/>
  <v:f eqn="prod @3 21600 pixelHeight"/>
  <v:f eqn="sum @0 0 1"/>
  <v:f eqn="prod @6 1 2"/>
  <v:f eqn="prod @7 21600 pixelWidth"/>
  <v:f eqn="sum @8 21600 0"/>
  <v:f eqn="prod @7 21600 pixelHeight"/>
  <v:f eqn="sum @10 21600 0"/>
 </v:formulas>
 <v:path o:extrusionok="f" gradientshapeok="t" o:connecttype="rect"/>
 <o:lock v:ext="edit" aspectratio="t"/>
</v:shapetype><v:shape id="Picture_x0020_7" o:spid="_x0000_i1035" type="#_x0000_t75"
 style='width:326.4pt;height:285.6pt;visibility:visible;mso-wrap-style:square'>
 <v:imagedata src="cassapa_tutorial_files/image001.png" o:title=""/>
</v:shape><![endif]--><![if !vml]><img width=544 height=476
src="cassapa_tutorial_files/image002.png" v:shapes="Picture_x0020_7"><![endif]></span></p>

<p class=MsoListParagraphCxSpFirst style='text-indent:-.25in;mso-list:l2 level1 lfo9'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]>A computer, to run the software</p>

<p class=MsoListParagraphCxSpMiddle style='text-indent:-.25in;mso-list:l2 level1 lfo9'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]>A camera, connected to the computer. It shall be
placed on the ceiling, exactly on top of the pool table, pointing 90 degrees to
the table top. Whenever possible, the camera lens shall be adjusted so the
table fills the whole camera images.</p>

<p class=MsoListParagraphCxSpLast style='text-indent:-.25in;mso-list:l2 level1 lfo9'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]>A projector, connected to the computer, to display
the augmented reality images over the table cloth. The projector is not
mandatory, since you can monitor the game detection using a remote laptop.
However, if you want to use the augmented reality resources of <span
class=SpellE>Cassapa</span>, you need a projector.</p>

<h1><span class=GramE>3.Installing</span> the software</h1>

<p class=MsoNormal>The installer provides two installing options: </p>

<p class=MsoListParagraphCxSpFirst style='text-indent:-.25in;mso-list:l4 level1 lfo4'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]>Install for the server</p>

<p class=MsoListParagraphCxSpLast style='text-indent:-.25in;mso-list:l7 level1 lfo3'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]>Install for the client </p>

<p class=MsoNormal>The server option shall be selected for installing the
software on the camera/projector computer. It will install both
cassapa_server_win.exe, cassapa_projector.exe and their related files. </p>

<p class=MsoNormal>The client option shall be selected when installing the
software for a user laptop, which will be used to remotely control the system.</p>

<p class=MsoNormal>Please note that all connections between these 3
applications are done via a local TCP/IP network, so they shall be installed
only on computers that are part of the same local network.</p>

<p class=MsoNormal>In order to install, run the installer and follow its
instructions. </p>

<p class=MsoNormal>Recommendations:</p>

<p class=MsoListParagraphCxSpFirst style='text-indent:-.25in;mso-list:l8 level1 lfo1'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]>Do not install it on Windows “Program files”
folder. <span class=SpellE>Cassapa</span> configuration files are stored along
with the executables, so it need to be installed on a writeable folder</p>

<p class=MsoListParagraphCxSpLast style='text-indent:-.25in;mso-list:l8 level1 lfo1'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]>Install using the same user login that will be
used for running the software</p>

<h1><span class=GramE>4.Operation</span></h1>

<p class=MsoNormal>After installing both the client and server, on different
computers, you shall start cassapa_server_win.exe on the camera/projector
computer. </p>

<p class=MsoNormal>The server shall detect the camera automatically, and start
grabbing frames from it. If more than a camera is available at the system, you
can change the active camera via the configuration file cassapa_server.ini (All
changes to <span class=SpellE>ini</span> files won’t be effective until the
related software is restarted)</p>

<p class=MsoNormal>This is the server GUI:</p>

<p class=MsoNormal align=center style='text-align:center'><span
style='mso-no-proof:yes'><!--[if gte vml 1]><v:shape id="Picture_x0020_2"
 o:spid="_x0000_i1034" type="#_x0000_t75" style='width:215.4pt;height:107.4pt;
 visibility:visible;mso-wrap-style:square'>
 <v:imagedata src="cassapa_tutorial_files/image003.png" o:title=""/>
</v:shape><![endif]--><![if !vml]><img width=359 height=179
src="cassapa_tutorial_files/image003.png" v:shapes="Picture_x0020_2"><![endif]></span></p>

<p class=MsoNormal>After making sure the server is running ok, launch the
software cassapa_client.exe on the remote control computer.</p>

<p class=MsoNormal>The client will try to find a server on the same network. If
it fails to find one, you’ll see the following image on client:</p>

<p class=MsoNormal align=center style='text-align:center'><span
style='mso-no-proof:yes'><!--[if gte vml 1]><v:shape id="Picture_x0020_3"
 o:spid="_x0000_i1033" type="#_x0000_t75" style='width:307.8pt;height:222pt;
 visibility:visible;mso-wrap-style:square'>
 <v:imagedata src="cassapa_tutorial_files/image004.png" o:title=""/>
</v:shape><![endif]--><![if !vml]><img width=513 height=370
src="cassapa_tutorial_files/image005.jpg" v:shapes="Picture_x0020_3"><![endif]></span></p>

<p class=MsoNormal>In this case, check your network connections and check if
server is running. After fixing the problems, you can force the client to try a
new connection by double-clicking any part of the client window. You’ll see the
client trying to find a new server:</p>

<p class=MsoNormal align=center style='text-align:center'><span
style='mso-no-proof:yes'><!--[if gte vml 1]><v:shape id="Picture_x0020_4"
 o:spid="_x0000_i1032" type="#_x0000_t75" style='width:308.4pt;height:222.6pt;
 visibility:visible;mso-wrap-style:square'>
 <v:imagedata src="cassapa_tutorial_files/image006.png" o:title=""/>
</v:shape><![endif]--><![if !vml]><img width=514 height=371
src="cassapa_tutorial_files/image007.jpg" v:shapes="Picture_x0020_4"><![endif]></span></p>

<p class=MsoNormal>After connecting, the client starts displaying the server
camera images, <span class=SpellE>overlayed</span> by server and game data. If
the server experiences any problem to capture the camera image, you’ll see the
following image on the client software:</p>

<p class=MsoNormal align=center style='text-align:center'><span
style='mso-no-proof:yes'><!--[if gte vml 1]><v:shape id="Picture_x0020_1"
 o:spid="_x0000_i1031" type="#_x0000_t75" style='width:316.2pt;height:228pt;
 visibility:visible;mso-wrap-style:square'>
 <v:imagedata src="cassapa_tutorial_files/image008.png" o:title=""/>
</v:shape><![endif]--><![if !vml]><img width=527 height=380
src="cassapa_tutorial_files/image009.jpg" v:shapes="Picture_x0020_1"><![endif]></span></p>

<p class=MsoNormal><o:p>&nbsp;</o:p></p>

<p class=MsoNormal>If everything runs ok, you shall see the pool table image on
the client window:</p>

<p class=MsoNormal align=center style='text-align:center'><span
style='mso-no-proof:yes'><!--[if gte vml 1]><v:shape id="Picture_x0020_5"
 o:spid="_x0000_i1030" type="#_x0000_t75" style='width:312pt;height:225.6pt;
 visibility:visible;mso-wrap-style:square'>
 <v:imagedata src="cassapa_tutorial_files/image010.png" o:title=""/>
</v:shape><![endif]--><![if !vml]><img width=520 height=376
src="cassapa_tutorial_files/image011.jpg" v:shapes="Picture_x0020_5"><![endif]></span></p>

<p class=MsoNormal>But note that the detection of the pool table, cue and balls
still depends on the correct configuration of the system, so it’s very unlikely
the detection will work out of the box. This depends on several factors like:</p>

<p class=MsoListParagraphCxSpFirst style='text-indent:-.25in;mso-list:l1 level1 lfo6'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]>The pool size and positioning</p>

<p class=MsoListParagraphCxSpMiddle style='text-indent:-.25in;mso-list:l1 level1 lfo6'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]>The pool cloth color</p>

<p class=MsoListParagraphCxSpMiddle style='text-indent:-.25in;mso-list:l1 level1 lfo6'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]>The cue color</p>

<p class=MsoListParagraphCxSpLast style='text-indent:-.25in;mso-list:l1 level1 lfo6'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]>The environment lighting</p>

<p class=MsoNormal>On the previous image, you can see that the system is
correctly detecting the cue and balls by checking the cue and ball icon on
top-left: These icons become lit green when detection is working, and become
gray when detection fails.</p>

<p class=MsoNormal>To configure your system, just double click any part of the
client window, while it is displaying the camera image. The configuration
window will pop-up:</p>

<p class=MsoNormal align=center style='text-align:center'><span
style='mso-no-proof:yes'><!--[if gte vml 1]><v:shape id="Picture_x0020_6"
 o:spid="_x0000_i1029" type="#_x0000_t75" style='width:200.4pt;height:131.4pt;
 visibility:visible;mso-wrap-style:square'>
 <v:imagedata src="cassapa_tutorial_files/image012.png" o:title=""/>
</v:shape><![endif]--><![if !vml]><img width=334 height=219
src="cassapa_tutorial_files/image013.jpg" v:shapes="Picture_x0020_6"><![endif]></span></p>

<p class=MsoNormal>This window has 7 buttons, from top-left to bottom-right:</p>

<p class=MsoListParagraphCxSpFirst style='text-indent:-.25in;mso-list:l0 level1 lfo7'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]>Configure the cue detector</p>

<p class=MsoListParagraphCxSpMiddle style='text-indent:-.25in;mso-list:l0 level1 lfo7'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]>Configure the ball detector</p>

<p class=MsoListParagraphCxSpMiddle style='text-indent:-.25in;mso-list:l0 level1 lfo7'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]>Configure the table detector</p>

<p class=MsoListParagraphCxSpMiddle style='text-indent:-.25in;mso-list:l0 level1 lfo7'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]>Configure the projector output</p>

<p class=MsoListParagraphCxSpMiddle style='text-indent:-.25in;mso-list:l0 level1 lfo7'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]>Toggle (on/off) the fine-detector (Uses more CPU
for a better detection)</p>

<p class=MsoListParagraphCxSpMiddle style='text-indent:-.25in;mso-list:l0 level1 lfo7'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]>Turn-off the server (You can use this to restart
the server, if you create a .BAT file to run it again forever whenever it
closes)</p>

<p class=MsoListParagraphCxSpLast style='text-indent:-.25in;mso-list:l0 level1 lfo7'><![if !supportLists]><span
style='font-family:Symbol;mso-fareast-font-family:Symbol;mso-bidi-font-family:
Symbol'><span style='mso-list:Ignore'>·<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></span></span><![endif]>Closes the configuration window</p>

<p class=MsoNormal>The first thing you shall configure is the table limits.
Open the configuration window and click the table configuration button to
display this image on the client window:</p>

<p class=MsoNormal align=center style='text-align:center'><span
style='mso-no-proof:yes'><!--[if gte vml 1]><v:shape id="Picture_x0020_8"
 o:spid="_x0000_i1028" type="#_x0000_t75" style='width:241.2pt;height:174.6pt;
 visibility:visible;mso-wrap-style:square'>
 <v:imagedata src="cassapa_tutorial_files/image014.png" o:title=""/>
</v:shape><![endif]--><![if !vml]><img width=402 height=291
src="cassapa_tutorial_files/image015.jpg" v:shapes="Picture_x0020_8"><![endif]></span></p>

<p class=MsoNormal>This will display the table limits the server considers
during game detection. Use the mouse to drag the top-left and bottom-right
limits, so the red rectangle on your system matches the internal borders of
your table, as displayed on the previous image. Please note that the camera
positioning and rotation may prevent you from getting a good rectangular image
from the table limits. After adjusting the positions, click the black box to
end this adjust.</p>

<p class=MsoNormal>After that, you shall configure the cue and balls detector.
This detector is based on several factors, <span class=SpellE>buyt</span> most
important are luminance and color, so you have to adjust the system according
to your table cloth, cue and balls. </p>

<p class=MsoNormal>Both cue and balls configuration is similar: Click the
corresponding <span class=SpellE>config</span> button to open the color adjust
window: </p>

<p class=MsoNormal align=center style='text-align:center'><span
style='mso-no-proof:yes'><!--[if gte vml 1]><v:shape id="Picture_x0020_9"
 o:spid="_x0000_i1027" type="#_x0000_t75" style='width:286.8pt;height:170.4pt;
 visibility:visible;mso-wrap-style:square'>
 <v:imagedata src="cassapa_tutorial_files/image016.png" o:title=""/>
</v:shape><![endif]--><![if !vml]><img width=478 height=284
src="cassapa_tutorial_files/image017.jpg" v:shapes="Picture_x0020_9"><![endif]></span></p>

<p class=MsoNormal>As commented before, detecting the cue and balls correctly
and fast is hard. So, the system is highly dependent on using the cue and table
cloth color to help the detection. These data is also highly dependent on the
ambient light, so it may be hard to get the best configuration at the first
time.</p>

<p class=MsoNormal>Your objective here is to leave only the cue or only the
balls visible. Everything white will be used by the detector, while the black
area will be ignored. So, you shall adjust the <span class=SpellE>trackbars</span>
on the left to leave only the cue or the balls (According to which one you’re
configuring) visible/white.</p>

<p class=MsoNormal>Note that, depending on your table cloth color or cue color,
it may be hard to separate these items on these configuration windows. In these
case, it’s recommended that you try a different cue color.</p>

<p class=MsoNormal>After this configuration, you shall be able to watch the
game detection on the client window:</p>

<p class=MsoNormal align=center style='text-align:center'><span
style='mso-no-proof:yes'><!--[if gte vml 1]><v:shape id="Picture_x0020_10"
 o:spid="_x0000_i1026" type="#_x0000_t75" style='width:312pt;height:225.6pt;
 visibility:visible;mso-wrap-style:square'>
 <v:imagedata src="cassapa_tutorial_files/image010.png" o:title=""/>
</v:shape><![endif]--><![if !vml]><img width=520 height=376
src="cassapa_tutorial_files/image018.jpg" v:shapes="Picture_x0020_10"><![endif]></span></p>

<p class=MsoNormal>If everything went fine, it’s time to start the
cassapa_projector.exe application on the server, and configure its output.</p>

<p class=MsoNormal>As soon as cassapa_projector.exe is started, it shall
connect to the server and start outputting the game aiming lines on the
projector. </p>

<p class=MsoNormal>It’s very unlikely that your projector will be positioned
exactly on the top of the table center, so you’ll probably need to adjust the
projector distortion. You can do this on the configuration window, pressing the
projector configuration button:</p>

<p class=MsoNormal align=center style='text-align:center'><span
style='mso-no-proof:yes'><!--[if gte vml 1]><v:shape id="Picture_x0020_11"
 o:spid="_x0000_i1025" type="#_x0000_t75" style='width:243pt;height:175.8pt;
 visibility:visible;mso-wrap-style:square'>
 <v:imagedata src="cassapa_tutorial_files/image019.png" o:title=""/>
</v:shape><![endif]--><![if !vml]><img width=405 height=293
src="cassapa_tutorial_files/image020.jpg" v:shapes="Picture_x0020_11"><![endif]></span></p>

<p class=MsoNormal>As soon as the projection configuration starts, you’ll see
that the projector starts displaying a calibration pattern (not displayed on
the image above), while the client window will display 4 numbered squares on
the image. </p>

<p class=MsoNormal>You can drag these 4 squares to adjust red rectangle so it
matches the test pattern projected on the table. After adjusting this, just
double-click any part of the client image to finish this configuration. </p>

</div>

</body>

</html>
