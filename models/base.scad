module make_base(){
    base_h = 25;
    inner_d = 65.5;
    thickness = 2;
    
    difference(){
        union(){
            // top
            if(1){
            difference(){
                translate([0,0,base_h])
                rotate([-45,0,0])
                translate([0,0,base_h/2]){
                    difference(){
                        cylinder(d=65.5+thickness*2, h=base_h, center=true, $fn=50);
                        cylinder(d=65.5-thickness*2, h=base_h*2, center=true, $fn=50);
                        
                        // clock cutout
                        color("red")
                        //translate([-10,0,0])
                        translate([0,0,base_h*1.5-10])
                        cylinder(d=65.5, h=base_h*2, center=true, $fn=50);
                    }
                }
                
                if(1)
                translate([0,0,base_h])
                rotate([-45/2,0,0])
                translate([0,0,-100/2])
                cube([300,300,100], center=true);
            }
            }
            
            // bottom
            if(1)
            intersection(){
                translate([0,0,base_h/2]){
                    difference(){
                        cylinder(d=65.5+thickness*2, h=base_h, center=true, $fn=50);
                        cylinder(d=65.5-thickness*2, h=base_h*2, center=true, $fn=50);
                    }
                }
                
                translate([0,0,base_h])
                rotate([-45/2,0,0])
                translate([0,0,-100/2])
                cube([300,300,100], center=true);
            }
        }
        
        // notch cutout
        translate([0,0,base_h])
        color("blue"){
            //translate([0,0,base_h-20/2])
            rotate([0,90,0])
            cylinder(d=20, h=100, center=true, $fn=50);
            
            //cube([10,10,10], center=true);
            
            rotate([-45,0,0])
            translate([0,-40*2/2,0])
            cube([200,40*2,20], center=true);
            
            rotate([35,0,0])
            translate([0,-40*2/2,60/2-20/2])
            cube([200,40*2,60], center=true);
        
        }

    }// end diff
    
    if(0)
    color("blue")
    translate([0,0,base_h])
    rotate([-45/2,0,0])
    translate([0,-40,0])
    scale([1,2,1])
    rotate([0,90,0])
    cylinder(d=50, h=100, center=true, $fn=50);

    // stabilization foot
    for(i=[-1:2:1])
    rotate([0,0,20*i])
    difference(){
        translate([0,65/2,0])
        rotate([-90-5,0,0])
        cylinder(d1=20, d2=0, h=100, $fn=50);
        translate([0,0,-100/2])
        cube([300,300,100], center=true);
    }

}

make_base();