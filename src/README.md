# directory src

## garlic-lib

The basic lib for the player without dependencies to the view component and for testing.
It integrates
- SMIL parser
- file classes for managing indexes, media, downloads, etc.
- system information for reports and inspecting
- generating reports and sends them to a WebDav-Address 
- configuration and loggings tools

## garlic-common

Some common classes which are needed for all the players

## garlic-widget

A Smil player based on Qt's widget technology (Windows, Linux, OS/X)

## garlic-c2qml

A Smil player based on Qt's QML technology controlled by C++ objects (Android and iOS)

## garlic-qml

A Smil player concept study based on pure QML technologie with javascript objects. Works only with images - Do not use it

## garlic-dummy

Only a minimal "client" for testing the garlic-lib (e.g. responsibility, memory leaks, etc.)



