$fn=100;
plate_thickness=1.6;
body_wall_thickness=2; //thickness of walls in mm
wall_thickness = 0.06; //thickness of the letter walls as delta offset.
letter_depth=25;//50;
support_height=25;
diffuser_thickness=0.8;
diffuser_border=10;
tol=0.12; //tolerance in percent
pin_height=2; //height of pins used in pin_mask and neg_pin_mask
module letters(){
scale([22,22,1]) linear_extrude(letter_depth) text("S&N", font = "Liberation Sans:style=Bold");
}
module N_shell(){
    scale([22,22,1]) union(){
    linear_extrude(plate_thickness) text("N", font = "Liberation Sans:style=Bold");
    linear_extrude(letter_depth) difference(){
    text("N", font = "Liberation Sans:style=Bold");
    offset(delta=-wall_thickness) text("N", font = "Liberation Sans:style=Bold");
}}}

module S_shell(){
    scale([22,22,1]) union(){
    linear_extrude(plate_thickness) text("S", font = "Liberation Sans:style=Bold");
    linear_extrude(letter_depth) difference(){
    text("S", font = "Liberation Sans:style=Bold");
    offset(delta=-wall_thickness) text("S", font = "Liberation Sans:style=Bold");
}}}

module amp_shell(){
    scale([22,22,1]) union(){
    linear_extrude(plate_thickness) text("&", font = "Liberation Sans:style=Bold");
    linear_extrude(letter_depth) difference(){
    text("&", font = "Liberation Sans:style=Bold");
    offset(delta=-wall_thickness) text("&", font = "Liberation Sans:style=Bold");
}}}
   

module screw(){
    //translating to y=198 inside this odule ensures same height in all letters
    translate([0,198,2.1]) rotate([180,0,0]) cylinder(d1=7,d2=3,h=4);
}

module corner_bracket_arduino(h=7.5){
    union(){
//        cube([43,19,15]);
        //cube();
    translate([3,-1,0]) cylinder(r=1,h=h);
    translate([3,20,0]) cylinder(r=1,h=h);
    translate([40,-1,0]) cylinder(r=1,h=h);
    translate([40,20,0]) cylinder(r=1,h=h);
        translate([44.5,12,0]) cylinder(r=1,h=h);
        translate([-1.5,6,0]) cylinder(r=1,h=h);
    }
}

module plug(){
    rotate([-90,0,0])cylinder(d=9.5,h=20);
}

//S shell with all holes and utilities
module S_bottom_final(){
//screw hole
union(){
    difference(){
    S_shell();
    translate([100,0,0])screw();
    translate([90,-5,12]) plug();
    translate([185,161,0]) difference(){
        cable_bridge(66.5,dove_scale=1+tol,z_scale=1.8);
        cube([2,1,10]);
    }
//    translate([0,0,letter_depth-pin_height/2]) S_pin_mask_neg();
}
    //arduino holder
//    color([1,0,0]) translate([25,37,body_wall_thickness])rotate([0,0,-35]) corner_bracket_arduino();
color([1,0,0]) translate([112,5,body_wall_thickness-0.5]) rotate([0,0,9]) corner_bracket_arduino();

}}

module pir(){
   cube([23.5,24.2,10]);
}

module pir_pins(dia=1.8){
   //pir();
    union(){
        color([1,0,0]) translate([-2.8,12.1,-(5.5-diffuser_thickness)]) cylinder(d=dia,h=5.5);
        color([0,1,0]) translate([26,12.1,-(5.5-diffuser_thickness)]) cylinder(d=dia,h=5.5);
    }
}

module S_diffuser(){
    difference(){
        scale([22,22,1]) union(){
         color("blue")translate([0,0,diffuser_border-diffuser_thickness])      
            linear_extrude(diffuser_thickness) offset(delta=+wall_thickness/2) text("S", font = "Liberation Sans:style=Bold");
        
    linear_extrude(diffuser_border) difference(){
    offset(delta=+wall_thickness) text("S", font = "Liberation Sans:style=Bold");
    offset(delta=+wall_thickness/2)text("S", font = "Liberation Sans:style=Bold");
        }  
      }
      translate([32,22,1]) rotate([0,0,-30]) pir();  
 }
 translate([32,22,diffuser_border-diffuser_thickness]) rotate([0,0,-30]) pir_pins(dia=1.6);
 }


module cable_bridge(length,height=10,depth_channel=1.8,dove_scale=1,z_scale=1){
    difference(){
            union(){
                cube([length,height,body_wall_thickness]);
                translate([0,0,body_wall_thickness]) cube([length,1,depth_channel]);
                translate([0,height-1,body_wall_thickness]) cube([length,1,depth_channel]);
        //dovetails
                scale([dove_scale,dove_scale,z_scale]) dove_tail();
                translate([length,0,2*z_scale]) rotate([0,180,0]) scale([dove_scale,dove_scale,z_scale]) dove_tail();
            }
            translate([0,0,body_wall_thickness+0.5]) dt_mask(length);
    }
}

module cable_bridge_cap(length,height=10){
    union(){
        cube([length,height,0.6]);
        translate([0,0,-1.2]) dt_mask(length);
    }
}

module dt_mask(length){
    translate([length/4,1.5,0.6]) scale([0.5,0.5,0.5]) rotate([0,0,90]) dove_tail();
    translate([3*length/4,10-1.5,0.6]) scale([0.5,0.5,0.5]) rotate([0,0,-90]) dove_tail();
}
module dove_tail(){
linear_extrude(body_wall_thickness) polygon([[0,4],[0,6],[-3,7],[-3,3]]);
}


module amp_diffuser_old(){
    union(){
        scale([22,22,1]) linear_extrude(diffuser_thickness) text("&", font = "Liberation Sans:style=Bold");
        translate([0,0,-pin_height/2+diffuser_thickness]) amp_pin_mask();
    }
}

module amp_diffuser(){
    scale([22,22,1]) union(){
    color("blue")translate([0,0,diffuser_border-diffuser_thickness])linear_extrude(diffuser_thickness) offset(delta=+wall_thickness/2) text("&", font = "Liberation Sans:style=Bold");
    linear_extrude(diffuser_border) difference(){
    offset(delta=+wall_thickness) text("&", font = "Liberation Sans:style=Bold");
    offset(delta=+wall_thickness/2)text("&", font = "Liberation Sans:style=Bold");
}}}


module amp_bottom_final(){
    difference(){
        amp_shell();
        translate([100,0,0])screw();
        translate([-14,161,0])cable_bridge(66.5,dove_scale=1+tol,z_scale=1.8);
        translate([163.5,161,0])cable_bridge(72,dove_scale=1+tol,z_scale=1.8);
//        translate([0,0,letter_depth-pin_height/2]) amp_pin_mask_neg();
    }
}

module N_bottom_final(){
        difference(){
            N_shell();
            translate([40,0,0])screw();
            translate([180,0,0])screw();
//            translate([0,0,letter_depth-pin_height/2]) N_pin_mask_neg();
            translate([-52.5,161,0])cable_bridge(72,dove_scale=1+tol,z_scale=1.8);
        }
}

module N_diffuser(){
    scale([22,22,1]) union(){
    color("blue")translate([0,0,diffuser_border-diffuser_thickness])linear_extrude(diffuser_thickness) offset(delta=+wall_thickness/2) text("N", font = "Liberation Sans:style=Bold");
    linear_extrude(diffuser_border) difference(){
    offset(delta=+wall_thickness) text("N", font = "Liberation Sans:style=Bold");
    offset(delta=+wall_thickness/2)text("N", font = "Liberation Sans:style=Bold");
}}}

//viz();

module viz(){
    //%letters();
    translate([0,0,letter_depth+2]) S_diffuser();
    S_bottom_final();
    translate([185,161,0]) difference(){
    cable_bridge(66.5);
    cube([2,3.5,10]);
    }
    
    translate([185,161,5]) cable_bridge_cap(66.5);
    translate([199,0,0]) amp_bottom_final();
    translate([199,0,letter_depth+2]) amp_diffuser();
    translate([362.5,161,0])cable_bridge(72);
    translate([362.5,161,5]) cable_bridge_cap(72);
    translate([415,0,0]) N_bottom_final();
    translate([415,0,letter_depth+2]) N_diffuser();
}
////////////////////////////
//parts to generate STL for:
////////////////////////////
//S_diffuser();
//amp_diffuser();
//N_diffuser();
//S_bottom_final();
//amp_bottom_final();
//N_bottom_final();
//cable_bridge(66.5);//for the space between S, &
//translate([0,15,0])cable_bridge_cap(66.5);
//translate([0,30,0])cable_bridge(72); //for the space between &, N
//translate([0,45,0])cable_bridge_cap(72);



//scale([22,22,1])  linear_extrude(0.3) offset(delta=-0.1) text("N", font = "Liberation Sans:style=Bold");

//scale([22,22,1])  linear_extrude(0.3) offset(delta=-0.1) text("S", font = "Liberation Sans:style=Bold");