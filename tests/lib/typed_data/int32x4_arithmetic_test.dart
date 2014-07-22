// Copyright (c) 2013, the Dart project authors.  Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
// VMOptions=--deoptimization_counter_threshold=1000 --optimization-counter-threshold=10

// Library tag to be able to run in html test framework.
library uint32x4_arithmetic_test;

import "package:expect/expect.dart";
import 'dart:typed_data';

testAdd() {
  var m = new Int32x4(0, 0, 0, 0);
  var n = new Int32x4(-1, -1, -1, -1);
  var o = m + n;
  Expect.equals(-1, o.x);
  Expect.equals(-1, o.y);
  Expect.equals(-1, o.z);
  Expect.equals(-1, o.w);

  m = new Int32x4(0, 0, 0, 0);
  n = new Int32x4(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
  o = m + n;
  Expect.equals(-1, o.x);
  Expect.equals(-1, o.y);
  Expect.equals(-1, o.z);
  Expect.equals(-1, o.w);

  n = new Int32x4(1, 1, 1, 1);
  m = new Int32x4(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
  o = m + n;
  Expect.equals(0, o.x);
  Expect.equals(0, o.y);
  Expect.equals(0, o.z);
  Expect.equals(0, o.w);

  n = new Int32x4(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
  m = new Int32x4(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
  o = m + n;
  Expect.equals(-2, o.x);
  Expect.equals(-2, o.y);
  Expect.equals(-2, o.z);
  Expect.equals(-2, o.w);

  n = new Int32x4(1, 0, 0, 0);
  m = new Int32x4(2, 0, 0, 0);
  o = n + m;
  Expect.equals(3, o.x);
  Expect.equals(0, o.y);
  Expect.equals(0, o.z);
  Expect.equals(0, o.w);

  n = new Int32x4(1, 3, 0, 0);
  m = new Int32x4(2, 4, 0, 0);
  o = n + m;
  Expect.equals(3, o.x);
  Expect.equals(7, o.y);
  Expect.equals(0, o.z);
  Expect.equals(0, o.w);

  n = new Int32x4(1, 3, 5, 0);
  m = new Int32x4(2, 4, 6, 0);
  o = n + m;
  Expect.equals(3, o.x);
  Expect.equals(7, o.y);
  Expect.equals(11, o.z);
  Expect.equals(0, o.w);

  n = new Int32x4(1, 3, 5, 7);
  m = new Int32x4(-2, -4, -6, -8);
  o = n + m;
  Expect.equals(-1, o.x);
  Expect.equals(-1, o.y);
  Expect.equals(-1, o.z);
  Expect.equals(-1, o.w);
}

testSub() {
  var m = new Int32x4(0, 0, 0, 0);
  var n = new Int32x4(1, 1, 1, 1);
  var o = m - n;
  Expect.equals(-1, o.x);
  Expect.equals(-1, o.y);
  Expect.equals(-1, o.z);
  Expect.equals(-1, o.w);

  o = n - m;
  Expect.equals(1, o.x);
  Expect.equals(1, o.y);
  Expect.equals(1, o.z);
  Expect.equals(1, o.w);
}

testShift() {
  var m = new Int32x4(1, 1, 1, 1);
  var n = m << 3;
  Expect.equals(8, n.x);
  Expect.equals(8, n.y);
  Expect.equals(8, n.z);
  Expect.equals(8, n.w);
  var o = m << 32;
  Expect.equals(o.x, m.x);
  Expect.equals(o.y, m.y);
  Expect.equals(o.z, m.z);
  Expect.equals(o.w, m.w);

  o = n >> 3;
  Expect.equals(1, o.x);
  Expect.equals(1, o.y);
  Expect.equals(1, o.z);
  Expect.equals(1, o.w);

  o = n >> 4;
  Expect.equals(0, o.x);
  Expect.equals(0, o.y);
  Expect.equals(0, o.z);
  Expect.equals(0, o.w);

  var p = new Int32x4(1, 1, 1, 1);
  p = p << 30;
  var i = 1;
  i = i << 30;
  Expect.equals(i, p.x);
  Expect.equals(i, p.y);
  Expect.equals(i, p.z);
  Expect.equals(i, p.w);

  m = (m | n) >> 32;
  Expect.equals(0, o.x);
  Expect.equals(0, o.y);
  Expect.equals(0, o.z);
  Expect.equals(0, o.w);
}

main() {
  for (int i = 0; i < 20; i++) {
    testAdd();
    testSub();
    testShift();
  }
}
