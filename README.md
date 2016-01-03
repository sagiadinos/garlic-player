# garlic-player

SMIL Player for Digital Signage
Goal is to create as most as needed SMIL 3.0 compatible player for Digital Signage proposes to run under Linux, Windows, OSX, android and maybe iOS

Compiled whith Qt 5.5. and will need and QtAV and ffmpeg. 

At the moment is an early alpha version which is able to player local SMIL-Files via Commandline
garlic-player -f path_to_smil_file

#Actual supported SMIL-Features
Playlist Elements:
seq, par

Media-Elements
video, audio, images, text (websites)

Attributes:
xml:id, region, fill, dur, begin, end

Layered multizone layouts:
root-layout, layout

Multizone attributes
regionName, top, left, width, height, z-index, backgroundColor