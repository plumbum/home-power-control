w1 = 90;
w2 = 94;

thickness = 1.6;

cube([w2, w2, 1]);

translate([2, 2, 1]) 
difference() {
    union() {
        cube([w1, w1, 2]);
        cube([12, 12, 7]);
        translate([w1-12, 0, 0]) cube([12, 12, 7]);
        translate([0, w1-12, 0]) cube([12, 12, 7]);
        translate([w1-12, w1-12, 0]) cube([12, 12, 7]);
    }
    translate([thickness,thickness,0])
        cube([w1-thickness*2, w1-thickness*2, 20]);
}
