var data = [
    {von: 0.62, son: 50, d: 31, soff: 19},
    {von: 1.04, son: 67, d: 31, soff: 36},
    {von: 1.45, son: 83, d: 31, soff: 52},
    {von: 1.85, son: 99, d: 31, soff: 68},
    {von: 2.25, son: 115, d: 31, soff: 84},
    {von: 2.63, son: 131, d: 30, soff: 101},
    {von: 3.02, son: 146, d: 30, soff: 116},
    {von: 3.41, son: 162, d: 31, soff: 131},
    {von: 3.77, son: 177, d: 31, soff: 146},
    {von: 4.14, son: 192, d: 30, soff: 162},
    {von: 4.53, son: 207, d: 31, soff: 176},
    {von: 4.86, son: 221, d: 31, soff: 190},
    {von: 5.21, son: 235, d: 30, soff: 205},
    {von: 5.56, son: 249, d: 30, soff: 219},
    {von: 5.9, son: 263, d: 30, soff: 233},
    {von: 6.25, son: 276, d: 30, soff: 246},
    {von: 6.56, son: 289, d: 29, soff: 260},
    {von: 6.89, son: 303, d: 30, soff: 273},
    {von: 7.21, son: 316, d: 30, soff: 286},
    {von: 7.54, son: 329, d: 30, soff: 299},
    {von: 7.85, son: 341, d: 29, soff: 312},
    {von: 8.16, son: 353, d: 29, soff: 324},
    {von: 8.48, son: 365, d: 28, soff: 337},
    {von: 8.78, son: 378, d: 29, soff: 349},
    {von: 9.07, son: 390, d: 29, soff: 361},
    {von: 9.37, son: 402, d: 29, soff: 373},
    {von: 9.68, son: 414, d: 29, soff: 385},
    {von: 9.96, son: 425, d: 28, soff: 397},
    {von: 10.25, son: 437, d: 28, soff: 409},
    {von: 10.53, son: 449, d: 29, soff: 420},
    {von: 10.86, son: 462, d: 29, soff: 433},
    {von: 11.18, son: 475, d: 29, soff: 446},
    {von: 11.49, son: 487, d: 28, soff: 459},
    {von: 11.8, son: 499, d: 28, soff: 471},
    {von: 12.08, son: 511, d: 28, soff: 483},
    {von: 12.4, son: 523, d: 28, soff: 495},
    {von: 12.63, son: 533, d: 27, soff: 506},
    {von: 12.92, son: 544, d: 27, soff: 517},
    {von: 13.18, son: 555, d: 26, soff: 529},
    {von: 13.48, son: 565, d: 26, soff: 539},
    {von: 13.7, son: 575, d: 25, soff: 550},
    {von: 13.94, son: 583, d: 23, soff: 560},
    {von: 14.15, son: 589, d: 20, soff: 569},
    {von: 14.31, son: 592, d: 16, soff: 576},
    {von: 14.43, son: 593, d: 12, soff: 581},
    {von: 14.51, son: 594, d: 9, soff: 585},
    {von: 14.58, son: 595, d: 7, soff: 588},
    {von: 14.63, son: 595, d: 5, soff: 590},
    {von: 14.66, son: 596, d: 5, soff: 591}
]

var dcoef = [
    2.7547296886729185e+001,
    2.8170254390869892e-001,
    -7.3601814488326277e-003,
    8.6834934413509122e-005,
    -5.4413842952499501e-007,
    1.9295945071995852e-009,
    -3.8892818872383331e-012,
    4.1551271392288520e-015,
    -1.8273629187776181e-018];


function polynom(coefs, x) {
    var y = coefs[0];
    console.log(0, y);
    for (var i = 1; i < coefs.length; i++) {
        var m = coefs[i] * x ** i;
        console.log(i, m);
        y += m;
    }
    return y;
}

var voncoef = [
    3.0107260077227869e+000,
    -1.9105429881738145e-001,
    5.2056152232194224e-003,
    -6.7619019633917313e-005,
    5.2989544848105892e-007,
    -2.6448227351266528e-009,
    8.5914943478029932e-012,
    -1.8073695723421046e-014,
    2.3720416849754977e-017,
    -1.7637858283739181e-020,
    5.6701628949511019e-024
];


// for (var i = 0; i < data.length; i++) {
//     const {von, son, soff, d} = data[i];
//     var val = -0.48954904741580652 + 0.023543515085681414 * son +  0.0000024361829796602023 * ( son * son );
// console.log(son, von, val, von - val);
// console.log(soff, d, soff + polynom(dcoef, soff), son);
// console.log(son, von);
// console.log(son, von, polynom(voncoef, son));
// }

// console.log('597', polynom(voncoef, 597));
// console.log('598', polynom(voncoef, 598));
// console.log('599', polynom(voncoef, 599));
console.log(polynom(dcoef, 570.48));


// 570.48 : 13.57
// 0 > 27.55
// 1 > 153.53
// 2 > -2186.16
// 3 > 14056.72
// 4 > -48005.98
// 5 > 92778.75
// 6 > -101917.42
// 7 > 59341.69
// 8 > -14223.19
// 570.48m25.48
