$fn = 20;
thickness = 1.2;
enc_hgt = 20;
pcb_ofs = 7;
pcb_thick = 1.6;
button_ofs = 3.5+1; button_d = 7;
led_ofs = 2; led_d = 2;
gap = 0.8;

difference() {
    bottom();
    translate([15.24, 19.685, -0.1]) {
        cylinder(h=thickness+0.2, d=23);
        translate([-9/2, 0, -0.1])
            cube([9, 5+23/2, thickness+0.3]);
    }
    
}

difference() {
    walls();
    translate([0,0, thickness+pcb_ofs]) pcb();
}

translate([5-thickness-gap, -5-thickness-gap, 0])
    hold();
translate([90-(5-thickness-gap), -5-thickness-gap, 0])
    hold();
translate([5-thickness-gap, 90-(-5-thickness-gap), 0])
    rotate([0,0,180]) hold();
translate([90-(5-thickness-gap), 90-(-5-thickness-gap), 0])
    rotate([0,0,180]) hold();

module hold() {
    difference() {
        union() {
            cylinder(d=10, h=thickness*2);
            translate([-5, 0, 0]) cube([10, 5, thickness*2]);
        }
        cylinder(d=3, h=thickness*2+0.2);
    }
}

module pcb() {
    // color("Green") cube([90, 90, 1.6]);
    color("Red") {
        translate([-5, 7.62, pcb_thick+led_ofs])
            rotate([0,90,0]) cylinder(h=10, d=led_d);
        translate([-5, 11.43, pcb_thick+led_ofs])
            rotate([0,90,0]) cylinder(h=10, d=led_d);
        translate([-5, 15.24, pcb_thick+led_ofs])
            rotate([0,90,0]) cylinder(h=10, d=led_d);
        translate([-5, 19.05, pcb_thick+led_ofs])
            rotate([0,90,0]) cylinder(h=10, d=led_d);
        translate([-5, 22.86, pcb_thick+led_ofs])
            rotate([0,90,0]) cylinder(h=10, d=led_d);
    }
    color("Black") {
        translate([-5, 30, pcb_thick+button_ofs])
            rotate([0,90,0]) cylinder(h=10, d=button_d);
        translate([-5, 40, pcb_thick+button_ofs])
            rotate([0,90,0]) cylinder(h=10, d=button_d);
        translate([-5, 60, pcb_thick+button_ofs])
            rotate([0,90,0]) cylinder(h=10, d=button_d);
    }
    color("Magenta") {
        translate([-5, 50, pcb_thick+7.5])
            rotate([0,90,0]) cylinder(h=10, d=5);
    }
    color("Blue") {
        translate([90-5, 10-10/2, pcb_thick+1])
            cube([10, 10, 5]);
        translate([90-5, 25-10/2, pcb_thick+1])
            cube([10, 10, 5]);
        translate([90-5, 40-10/2, pcb_thick+1])
            cube([10, 10, 5]);
        translate([90-5, 55-10/2, pcb_thick+1])
            cube([10, 10, 5]);
        translate([90-5, 70-10/2, pcb_thick+1])
            cube([10, 10, 5]);
    }
    color("Gold") {
        translate([-5, 75-13/2, pcb_thick+0.5])
            cube([10, 13, 12]);
    }
}

module walls() {
    difference() {
        translate([-thickness-gap, -thickness-gap, 0])
            cube([90+thickness*2+gap*2,
                90+thickness*2+gap*2,
                enc_hgt+thickness*2+pcb_ofs]);
        translate([-gap, -gap, -0.1])
            cube([90+gap*2, 90+gap*2, enc_hgt+thickness*2+pcb_ofs+0.2]);
    }
    
}

module bottom() {
    translate([-gap, -gap, 0])
        cube([90+gap*2, 90+gap*2, thickness]);
    translate([0,0,thickness]) screw_mount();
    translate([90-6,0,thickness]) screw_mount();
    translate([0,90-6,thickness]) screw_mount();
    translate([90-6,90-6,thickness]) screw_mount();
}

module screw_mount() {
    difference() {
        cube([6, 6, pcb_ofs]);
        translate([3,3,0]) cylinder(h=pcb_ofs+0.1, d=1.8);
    }
}
