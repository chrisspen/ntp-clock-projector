
module light_tube_row(d=0.8, width=10, height=10, gap=0.5){
    for(i=[0:width/(d+gap)-d/2]){
        translate([(d+gap)*i + d/2,0,0])
        cylinder(d=d, h=height, center=true, $fn=50);
    }
}

module light_mask_digit(thickness=1.5, width=4.7, buffer=1, height=10){
    
    // vert
    for(j=[-1:2:1])
    for(i=[-1:2:1])
    translate([(width/2+thickness/2)*j,(width/2+thickness/2)*i,0])
    cube([width,thickness-buffer/2,height], center=true);
    
    // horz
    if(1)
    for(i=[-1:1:1])
    translate([(width+thickness)*i,0,0])
    translate([0,0,0])
    cube([thickness-buffer/2,width,height], center=true);
}

module light_mask_digit_tubes(thickness=1.5, width=4.7, buffer=1, height=10, hole_d=1.2){
    
    gap = 0.5;
    span = width/(hole_d+gap)-hole_d/2;
    trim = span - floor(span);
    
    // vert
    for(j=[-1:2:1])
    for(i=[-1:2:1])
    translate([(width/2+thickness/2)*j,(width/2+thickness/2)*i,0])
    translate([-width/2+trim/2,0,0])
    light_tube_row(d=hole_d, gap=0.4, height=50, width=width, gap=gap);
    //cube([width,thickness-buffer/2,height], center=true);
    
    // horz
    if(1)
    for(i=[-1:1:1])
    translate([(width+thickness)*i,0,0])
    translate([0,-width/2+trim/2,0])
    rotate([0,0,90])
    light_tube_row(d=hole_d, gap=0.4, height=50, width=width, gap=gap);
    //cube([thickness-buffer/2,width,height], center=true);
}

module light_mask(height=20){
    difference(){
        // main mass
        cube([15+2,10+2,height], center=true);
        
        // single digit cutout
//        color("red"){
//            translate([0,0,0])
//            light_mask_digit(height=height*2, buffer=0);
//        }
        // single digit cutout
        color("blue"){
            translate([0,0,0])
            light_mask_digit_tubes(height=height*2, buffer=0);
        }
    }// end diff
    
    if(0)
    color("red")
    translate([0,0,0])
    light_mask_digit(buffer=0);
}

light_mask();

// real digit size
if(0)
color("orange")
translate([0,0,-40])
cube([15,10,1], center=true);
