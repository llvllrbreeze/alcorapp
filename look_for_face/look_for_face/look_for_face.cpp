#include "cv.h"
#include "highgui.h"
#include "face_finder.h"

#include <stdio.h>
#include <stdlib.h>

void main()
{
	
	FaceFinder finder;
	finder.init();

	finder.findFace(40);

}