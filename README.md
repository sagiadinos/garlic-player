# garlic-player

SMIL Player for Digital Signage

The goal is to create an as most SMIL 3.0 compatible as needed multimedia player for Digital Signage proposes to run primary under Linux, Windows, OSX. Maybe Android and iOS for later.

Compiled whith Qt 5.5. and QtAV 1.8.0

At the moment we have an early alpha version which is able to player local SMIL-Files via command line under Linux.

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