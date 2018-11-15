use <openscad-extra/src/countersink.scad>;
use <Gears.scad>;

module bracket(width=5){
    linear_extrude(height=width)
    polygon(points=[[0,0],[2,0],[2,1],[1,1]], paths=[[0,1,2,3]]);
}

module nodemcu_board_bb(buffer=0){
    cube([26+buffer,2.4,48], center=true);
}

module led_backpack_holes(d=2){
    color("red")
    for(i=[-1:2:1])for(j=[-1:2:1])
    translate([i*((41.75+2)/2),j*(20.92/2+1),0])
    cylinder(d=d, h=100, center=true, $fn=50);
}

module led_backpack(face_extend=0, face_buffer=0){
    difference(){
        union(){
            translate([0,0,8/2+2/2+face_extend/2])
            cube([51+face_buffer,20+face_buffer,8+face_extend], center=true);//segment display
            
            color("green")
            cube([51,27,2], center=true);//board
        }
        led_backpack_holes();
    }
    
    color("blue")
    translate([0,11.5,-(3/2+2/2)])
    cube([12,2.5,3], center=true);//header
    
}

module clock_housing(){
    d = 65;
    h = 67;
    wall_thickness = 3;
    gear_d = 68;

    difference(){
        union(){
            difference(){
                translate([0,0,-h/2+18])
                difference(){
                    // main mass
                    cylinder(d=d, h=h, center=true, $fn=100);
                    
                    // interior cutout
                    cylinder(d=d-wall_thickness*2, h=h*2, center=true, $fn=100);
                }
                
                // bottom lie cutout
                color("blue")
                translate([0,0,-50+1-0.1])
                housing_lid(buffer=0.5);
            }
            
            if(1){
                // bottom axle holder
                if(1)
                color("red")
                translate([0, 35.5-3, 41.1-47])
                scale([1,1.4,1])
                cylinder(d1=0, d2=15, h=10, center=true, $fn=100);
                
                // top axle holder
                intersection(){
                    color("red")
                    translate([0, 35.5-3, 41.1-21.5])
                    scale([1,1.4,1])
                    mirror([0,0,1])
                    cylinder(d1=0, d2=15, h=10, center=true, $fn=100);
                    
                    color("green")
                    translate([0,0,14.5+3.5/2])
                    cylinder(d=100, h=3.5, center=true);
                }
            }
                        
            // mass for rack shaft
            translate([0, 29-5/2-.5, -15.5+1+.2]){
                cube([17, 10, h-2.4], center=true);
            }

            // clock face
            if(1)
            color("green")
            translate([0,0,15.5])
            housing_lid(buffer=0.5);

            color("red")
            translate([gear_d,0,-30])
            worm_gear(
                modul=1,
                tooth_number=gear_d,
                gangzahl=2,
                width=8,
                length=20,
                bohrung_schnecke=4,
                bohrung_rad=gear_d-5,
                pressure_angle=20,
                gradient_angle=10,
                optimized=1,
                zusammen_gebaut=0,
                show_spur=1,
                show_worm=0);
            difference(){
                union(){
                    translate([0,0,-2.5/2-20.75])
                    cylinder(d1=70, d2=65, h=2.5, $fn=100);
                    translate([0,0,-2.5/2-31.25])
                    cylinder(d1=65, d2=70, h=2.5, $fn=100);
                }
                color("red")
                translate([0,0,-30])
                cylinder(d=59.5, h=30, center=true, $fn=100);
            }

            // led mount mass
            difference(){
                intersection(){
                    cylinder(d=d, h=h, center=true, $fn=100);
                    color("blue")
                    for(i=[-1:2:1])
                    translate([-28*i,0,14])
                    cube([20,27,8-.1], center=true);
                }
                translate([0,0,-35])
                led_backpack_holes(d=2);
            }//diff
            
            intersection(){
                // nodemcu top mount mass
                for(i=[0:1])
                color("pink")
                mirror([i,0,0])
                translate([8.25,-30+.25,-34.5])
                scale([1,6,6])
                rotate([0,90,0])
                bracket(width=5);
                
                cylinder(d=d, h=100, center=true, $fn=100);
            }
            
        }//union
       
        // clock face cutout
        if(1)
        translate([0,0,10-1])
        led_backpack(face_extend=1, face_buffer=0.5);

        // nodemcu top mount holes
        for(i=[-1:2:1])
        color("red")
        translate([10.5*i,-24,-43.5])
        rotate([90,0,0])
        cylinder(d=2, h=10, center=true, $fn=40);
        
        // rst hole
        color("red")
        translate([-6.5,-35,-43])
        rotate([90,0,0])
        cylinder(d=2, h=20, center=true, $fn=50);

        // cutout hole for rack gear
        scale([1,1,3])
        translate([0,0,23+50])
        //translate([0,0,25.4])
        translate([-10/2, 29+0, -85/2-8.2]){
            lens_holder_rack(bb=1, length=85, buffer=0.5);
        }

        // worm gear cutout
        if(1)
        translate([0,35.5,41.1-27])
        lens_holder_worm_gear(bb=1, buffer=1);

        // axle hole
        color("red")
        translate([0, 35.5, 15/2-1/2+11])
        //cylinder(d=1, h=15, center=true, $fn=100);
        make_countersink(d1=2.5, d2=5, inner=30);
            
        // hood mount holes
        if(0)
        for(i=[-1:2:1])
        color("red")
        translate([0,0,15.5])
        rotate([0,0,20*i])
        rotate([0,90,0])
        cylinder(d=2.5, h=100, center=true, $fn=100);
        
        // vertical hood mount holes
        color("red")
        for(i=[0:3])
        rotate([0,0,90*i+45])
        translate([0,26.5,10])
        cylinder(d=2, h=20, center=true, $fn=100);

    }//diff

    rotate([0,0,0]){

        // nodemcu bottom holder slot cutout
        difference(){
            for(i=[0:1])
            color("red")
            mirror([i,0,0])
            translate([10,-28,14])
            scale([1,8,8])
            rotate([0,90,0])
            bracket(width=5);
            
            color("purple")
            translate([0,-22.95,-21.4])
            nodemcu_board_bb(buffer=0.5);
        }

    }

    difference(){
        for(i=[0:3])
        color("pink")
        rotate([0,0,45+90*i])
        translate([-5/2,-30,-34.5])
        scale([1,6,6])
        rotate([0,90,0])
        bracket(width=5);

        translate([0,0,-49])
        lid_mount_holes();
    }
    
}

module lens_holder_worm_gear(bb=0, buffer=0, axle_d=3){
    if(bb){
        color("red")
        translate([0,0,-14.4/2])
        cylinder(d=13.5+buffer, h=14.4+buffer, center=true, $fn=50);
    }else{
        //rotate([0,180,0])
        scale([1,1,1-.04])
        mirror([0,0,1])
        worm(
            modul=1,
            gangzahl=2+2,
            length=15,
            bohrung=axle_d,//inner hole
            pressure_angle=20,
            gradient_angle=20,
            zusammen_gebaut=true);
    }
}

module lid_mount_holes(){
    for(i=[0:3])
    rotate([0,0,45+90*i])
    translate([0,-26.5,0])
    rotate([180,0,0])
    make_countersink(d1=2, d2=3.75, h=1.5, inner=10);
}

module housing_lid(buffer=0, usb_cutout=0, mount_holes=0){
    notch_s = 2-buffer;
    difference(){
        translate([0,0,2/2+buffer/2])
        cylinder(d=59+1*2+buffer*2, h=2+buffer, center=true, $fn=100);
        
        if(usb_cutout)
        color("red")
        translate([0,-25,0])
        cube([12,7,20], center=true);
    
        if(mount_holes){
            translate([0,0,0])
            lid_mount_holes();
        
            for(i=[0:2])
            color("red")
            rotate([0,0,-90+90*i])
            translate([0,61.5/2+.2,0])
            rotate([0,0,45])
            cube([notch_s,notch_s,5], center=true);
        }
            
    }
    
}

module lens(){
    //cube([85, 54, .5], center=true);
    color("green")
    cylinder(d=54, h=10, center=true, $fn=100);
}

module lens_holder_rack(bb=0, length=85, buffer=0){
    difference(){
        union(){
            if(bb){
                color("red")
                translate([10/2,-5/2+1/2,1.1])
                cube([10+buffer, 7+buffer, length], center=true);
            }else{
                rotate([0,90,0])
                rack(
                    modul=1,
                    length=length,
                    hoehe=5,
                    width=10,
                    pressure_angle=20,
                    gradient_angle=20
                );
            }
            
            color("blue")
            for(i=[0:1])
            translate([10*i,-3,1.1])
            rotate([0,0,45])
            cube([2.6+buffer, 2.6+buffer, length], center=true);
            
            // mount block mass
            color("red")
            translate([10/2,-5/2+1/2,1.1+37.51])
            cube([10, 6, 10], center=true);
        
        }//union
        
        // mount hole
        translate([5,-1,25+10])
        cylinder(d=2.5, h=20, $fn=100);
    }
    
    
}

module lens_holder(notch=0){
    rack_length = 85;
    
    difference(){
        difference(){
            translate([0,0,.25])
            difference(){
                // main mass
                cylinder(d=65, h=15, center=true, $fn=100);
                
                // lens cutout
                cylinder(d=53.75, h=10+1.5, center=true, $fn=100);
                
                // view area cutout
                cylinder(d=48, h=20, center=true, $fn=100);

                // half cutoff
                if(1)
                translate([0,0,100/2])
                cube([100,100,100], center=true);
            }
            
            if(notch)
            translate([-10/2, 29+0, -rack_length/2-8.2 + 1]){
                lens_holder_rack(bb=1, buffer=0.5);
            }
        }//diff
        
        color("red")
        for(i=[0:2])
        translate([0,0,(1-notch)*-10])
        rotate([0,0,90*i-180])
        translate([(65-7)/2,0,-7.25])
        rotate([180,0,0])
        make_countersink(d1=2.4, d2=5, inner=20);
        
        color("blue")
        rotate([0,0,90])
        translate([(65-9)/2,0,-7.25+(notch)*-10])
        rotate([180,0,0])
        make_countersink(d1=2.4, d2=5, inner=20);
    }
    
    if(0)
            translate([-10/2, 29+0, -rack_length/2-8.2 + 1 -10]){
                lens_holder_rack(bb=0, buffer=0);
            }
}

module lens_holder_top(){
    lens_holder(notch=0);
}

module lens_holder_bottom(){
    lens_holder(notch=1);
}

// position A
if(0)
translate([0,0,-20])
rotate([0,0,90])
import("electronics/NodeMCU.stl");

// position B
if(0)
translate([0,15,-20])
rotate([0,0,180])
rotate([90,0,0])
rotate([0,0,90])
import("electronics/NodeMCU.stl");

// position C
if(0)
translate([0,12.25,-20-1.75])
rotate([0,0,0])
rotate([90,0,0])
rotate([0,0,90])
import("electronics/NodeMCU.stl");

// position D ////////////////////////////
if(0)
//translate([0,0,-20])
translate([0,12.25-25,-20-1.75])
rotate([0,0,0])
rotate([90,0,0])
rotate([0,0,90])
color("green")
import("electronics/NodeMCU.stl");

if(0)
//color("blue")
translate([0,0,10-1])
led_backpack();

if(1)
difference(){
    //color("orange")
    translate([0,0,-0])
    clock_housing();
    if(0)
    translate([200/2,0,0])
    cube([200,200,200], center=true);
    if(0)
    translate([0,200/2,0])
    cube([200,200,200], center=true);
}

// 3x lens needs to be 7.25" from front face of LED=184.15mm
// 10x lens needs to be ~2" from front face of LED=50mm
if(0)
for(i=[-1:1:1])
translate([0,0,23+50+25.4*i])
lens();

if(1)
color("orange")
translate([0,0,-50+1-15])
housing_lid(buffer=0, usb_cutout=1, mount_holes=1);

if(0)
scale([10,10,10])
bracket(width=5);

if(1){
    translate([0,0,23+50+20])
    rotate([0,0,180])
    rotate([180,0,0])
    lens_holder_top();
    translate([0,0,23+50])
    lens_holder_bottom();
}

if(1)
translate([0,35.5,41.1-27])
lens_holder_worm_gear(bb=0);
