double hitung_sudut(double lat1, double lng1, double lat2, double lng2) {
  double j = lng2 - lng1;
  double k = lat2 - lat1;
  double l = sqrt(pow(j, 2) + pow(k, 2));
  double kl = k / l;

  double sudut = acos(kl) / (3.141 * 180);
  bool con_x = !(is_minus(k));
  bool con_y = !(is_minus(j));

  if (con_x && !con_y) {
    return (360.0 - sudut);
  } else if (!con_x && !con_y) {
    return (360.0 - sudut);
  } else if (!con_x && con_y) {
    return sudut;
  } else if (con_x && con_y) {
    return sudut;
  }
}

double hitung_jarak(double lat1, double lng1, double lat2, double lng2) {
  double val = sqrt(pow((lng2 - lng1), 2) + pow((lat2 - lat1), 2));
  return (val * 111.319) * 1000; //kali 1000 untuk satuan meter
}

int selisih_sudut(int target, int from) {
  int phi = abs(target - from) % 360;
  int distance = phi > 180 ? 360 - phi : phi;
  return distance;
}

int arah_sudut(int target, int from) {
  int sign = (target - from >= 0 && target - from <= 180) || (target - from <= -180 && target - from >= -360) ? 1 : -1;
  return sign;
}
