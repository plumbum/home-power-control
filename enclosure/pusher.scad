$fn=50;

difference() {
    union() {
        cylinder(h=4, d=5.5);
        cylinder(h=1, d=7);
    }
    cylinder(h=3, d=3.5);
}

