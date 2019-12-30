
int zOff = 150;
int xOff = 0;
int yOff = 0;
int cSize = 50;
int view_plane = 64;
float angle = PI/60;

float cube3d[8][3] = {
  {xOff - cSize,yOff + cSize,zOff - cSize},
  {xOff + cSize,yOff + cSize,zOff - cSize},
  {xOff - cSize,yOff - cSize,zOff - cSize},
  {xOff + cSize,yOff - cSize,zOff - cSize},
  {xOff - cSize,yOff + cSize,zOff + cSize},
  {xOff + cSize,yOff + cSize,zOff + cSize},
  {xOff - cSize,yOff - cSize,zOff + cSize},
  {xOff + cSize,yOff - cSize,zOff + cSize}
};
unsigned char cube2d[8][2];



/*
void spincube () {
    zOff = 150;
    view_plane = 64;
    int rsteps = random(10,600);
    switch(random(12)) {
      case 0:
        for (int i = 0; i < rsteps; i++) {
          zrotate(angle);
          printcube();
        }
        break;
      case 1:
        for (int i = 0; i < rsteps; i++) {
          zrotate(2*PI - angle);
          printcube();
        }
        break;
      case 2:
        for (int i = 0; i < rsteps; i++) {
          xrotate(angle);
          printcube();
        }
        break;
      case 3:
        for (int i = 0; i < rsteps; i++) {
          xrotate(2*PI - angle);
          printcube();
        }
        break;
      case 4:
        for (int i = 0; i < rsteps; i++) {
          yrotate(angle);
          printcube();
        }
        break;
      case 5:
        for (int i = 0; i < rsteps; i++) {
          yrotate(2*PI - angle);
          printcube();
        }
      case 6:
      case 7:
      case 8:
        for (int i = 0; i < rsteps; i++) {
          view_plane+=random(2);
          if (view_plane>=96) 
            view_plane = 0;
          zOff--;
          if (zOff<=0) 
            zOff = 150;
          printcube();
        }
        break;
      default:
        for (int i = 0; i < rsteps; i++) {
          capture_enabled = i%2;
          yrotate(2*PI - angle);
          xrotate(2*PI - angle);
          zrotate(2*PI + angle);
          printcube();
        }
      break;
    }

    //capture();
    draw_cube();
}*/

void printcube() {
  //calculate 2d points
  for(byte i = 0; i < 8; i++) {
    cube2d[i][0] = (unsigned char)((cube3d[i][0] * view_plane / cube3d[i][2]) + (TV.hres()/2));
    cube2d[i][1] = (unsigned char)((cube3d[i][1] * view_plane / cube3d[i][2]) + (TV.vres()/2));
  }
  //TV.delay_frame(1);
  //TV.clear_screen();

  //if (capture_enabled)
  //  capture();

  draw_cube();
}

void zrotate(float q) {
  float tx,ty,temp;
  for(byte i = 0; i < 8; i++) {
    tx = cube3d[i][0] - xOff;
    ty = cube3d[i][1] - yOff;
    temp = tx * cos(q) - ty * sin(q);
    ty = tx * sin(q) + ty * cos(q);
    tx = temp;
    cube3d[i][0] = tx + xOff;
    cube3d[i][1] = ty + yOff;
  }
}

void yrotate(float q) {
  float tx,tz,temp;
  for(byte i = 0; i < 8; i++) {
    tx = cube3d[i][0] - xOff;
    tz = cube3d[i][2] - zOff;
    temp = tz * cos(q) - tx * sin(q);
    tx = tz * sin(q) + tx * cos(q);
    tz = temp;
    cube3d[i][0] = tx + xOff;
    cube3d[i][2] = tz + zOff;
  }
}

void xrotate(float q) {
  float ty,tz,temp;
  for(byte i = 0; i < 8; i++) {
    ty = cube3d[i][1] - yOff;
    tz = cube3d[i][2] - zOff;
    temp = ty * cos(q) - tz * sin(q);
    tz = ty * sin(q) + tz * cos(q);
    ty = temp;
    cube3d[i][1] = ty + yOff;
    cube3d[i][2] = tz + zOff;
  }
}

void viewplane(int amt) {
  view_plane+=amt;
  if (view_plane>=96) 
    view_plane = 0;
}

void draw_cube() {
  TV.draw_line(cube2d[0][0],cube2d[0][1],cube2d[1][0],cube2d[1][1],WHITE);
  TV.draw_line(cube2d[0][0],cube2d[0][1],cube2d[2][0],cube2d[2][1],WHITE);
  TV.draw_line(cube2d[0][0],cube2d[0][1],cube2d[4][0],cube2d[4][1],WHITE);
  TV.draw_line(cube2d[1][0],cube2d[1][1],cube2d[5][0],cube2d[5][1],WHITE);
  TV.draw_line(cube2d[1][0],cube2d[1][1],cube2d[3][0],cube2d[3][1],WHITE);
  TV.draw_line(cube2d[2][0],cube2d[2][1],cube2d[6][0],cube2d[6][1],WHITE);
  TV.draw_line(cube2d[2][0],cube2d[2][1],cube2d[3][0],cube2d[3][1],WHITE);
  TV.draw_line(cube2d[4][0],cube2d[4][1],cube2d[6][0],cube2d[6][1],WHITE);
  TV.draw_line(cube2d[4][0],cube2d[4][1],cube2d[5][0],cube2d[5][1],WHITE);
  TV.draw_line(cube2d[7][0],cube2d[7][1],cube2d[6][0],cube2d[6][1],WHITE);
  TV.draw_line(cube2d[7][0],cube2d[7][1],cube2d[3][0],cube2d[3][1],WHITE);
  TV.draw_line(cube2d[7][0],cube2d[7][1],cube2d[5][0],cube2d[5][1],WHITE);
}
