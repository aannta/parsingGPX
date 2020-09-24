This program demonstrates the use of C control flow constructs, C I/O functions, to implement a state machine. 

The program extract GPS trackpoint information from a GPX file read from standard input. It returns to standard output values of lat and lon attributes of the trkpt elements together with the text of the ele and time elements contained in each trkpt element. Where the time element's text contains commas, it replaces them with &comma.

Example Input:
<?xml version="1.0" encoding="UTF-8"?>
<gpx creator="StravaGPX" version="1.1" xmlns="http://www.topografix.com/GPX/1/1" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd">
 <metadata>
  <time>2018-08-24T13:49:45Z</time>
 </metadata>
 <trk>
  <name>Morning Ride</name>
  <type>1</type>
  <trkseg>
   <trkpt lat="41.3078680" lon="-72.9342120">
    <ele>20.0</ele>
    <time>2018-08-24T13:49:45Z</time>
   </trkpt>
   <trkpt lat="41.3078680" lon="-72.9342120">
    <ele>20.0</ele>
    <time>2018-08-24T13:49:46Z</time>
   </trkpt>
   <trkpt lat="41.3078810" lon="-72.9342590">
    <ele>20.0</ele>
    <time>2018-08-24T13:49:49Z</time>
   </trkpt>
  </trkseg>
 </trk>
</gpx>
  
Example Output:
41.3078680,-72.9342120,20.0,2018-08-24T13:49:45Z
41.3078680,-72.9342120,20.0,2018-08-24T13:49:46Z
41.3078810,-72.9342590,20.0,2018-08-24T13:49:49Z