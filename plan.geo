h=0.1;
d=0.4;
xmin=0;
xmax=4+d;
ymin=0;
ymax=2;
// Box
Point(1) = {xmin-d, ymin-d, 0,h};
Point(2) = {xmax+d, ymin-d, 0,h};
Point(3) = {xmax+d, ymax+d, 0,h};
Point(4) = {xmin-d, ymax+d, 0,h};

// box
Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 1};

// surfaces
Line Loop(38) = {4, 1, 2, 3};
Plane Surface(43) = {38};

// Physical
Physical Line("box") = {1,2,3,4};

Physical Surface("domain") = {43};

