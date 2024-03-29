/* shadow.h - header file contain shadow generating proceedures
	
	All of the code here was provided by Penny de Byl in tutorial eight of CSC3406

*/
#ifndef __SHADOW_H__
#define __SHADOW_H__

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double L[4] = { 0, 8, 0, 0 };   /* Light location. */
double O[3] = { 0,0,0};      /* Cut-out origin. */
double M[3], N[3];            /* Vectors to orient the cut-out. */
double Pbg[4];                /* Plane below the true ground plane. */
double Pg[4];                 /* Plane at the actual ground plane. */

/* Three vertices just below the ground plane. */
//double Sbg[3] = { 0, -1.5, 0 };

double Sbg[3] = { 0, -0.5, 0 };
double Tbg[3] = { 0, -0.5, 1 };
double Rbg[3] = { 1, -0.5, 0 };
/* Three vertices on the ground plane. */
double Sg[3] = { 0, 0, 0 };
double Tg[3] = { 0, 0, 1 };
double Rg[3] = { 1, 0, 0 };


enum {
	X, Y, Z, W  /* Used to make code involving coordinates more readable. */
};


/* Create a matrix that will project the desired shadow. */
void shadowMatrix(GLfloat shadowMat[4][4], GLdouble groundplane[4], GLfloat lightpos[4])
{
	GLfloat dot;

	/* Find dot product between light position vector and ground plane normal. */
	dot = groundplane[X] * lightpos[X] +
		groundplane[Y] * lightpos[Y] +
		groundplane[Z] * lightpos[Z] +
		groundplane[W] * lightpos[W];

	shadowMat[0][0] = dot - lightpos[X] * groundplane[X];
	shadowMat[1][0] = 0.f - lightpos[X] * groundplane[Y];
	shadowMat[2][0] = 0.f - lightpos[X] * groundplane[Z];
	shadowMat[3][0] = 0.f - lightpos[X] * groundplane[W];

	shadowMat[X][1] = 0.f - lightpos[Y] * groundplane[X];
	shadowMat[1][1] = dot - lightpos[Y] * groundplane[Y];
	shadowMat[2][1] = 0.f - lightpos[Y] * groundplane[Z];
	shadowMat[3][1] = 0.f - lightpos[Y] * groundplane[W];

	shadowMat[X][2] = 0.f - lightpos[Z] * groundplane[X];
	shadowMat[1][2] = 0.f - lightpos[Z] * groundplane[Y];
	shadowMat[2][2] = dot - lightpos[Z] * groundplane[Z];
	shadowMat[3][2] = 0.f - lightpos[Z] * groundplane[W];

	shadowMat[X][3] = 0.f - lightpos[W] * groundplane[X];
	shadowMat[1][3] = 0.f - lightpos[W] * groundplane[Y];
	shadowMat[2][3] = 0.f - lightpos[W] * groundplane[Z];
	shadowMat[3][3] = dot - lightpos[W] * groundplane[W];
}

typedef GLdouble FPvalue;
#define constructShadowVolumeMatrix constructShadowVolumeMatrixd
void constructShadowVolumeMatrix(FPvalue P[5][3], FPvalue M[4][4])
{
	FPvalue t1, t2, t3, t4, t5, t6, t7, t9, t10, t11, t12, t13, t15, t16, t17, t18;
	FPvalue t19, t21, t22, t25, t28, t30, t31, t34, t36, t37, t38, t39, t40, t41;
	FPvalue t42, t43, t44, t45, t46, t47, t48, t50, t51, t53, t54, t55, t56, t58;
	FPvalue t59, t60, t61, t65, t66, t67, t68, t69, t70, t71, t72, t74, t75, t76;
	FPvalue t77, t78, t79, t80, t81, t82, t83, t84, t85, t86, t87, t88, t89, t90;
	FPvalue t93, t94, t95, t96, t97, t98, t99, t100, t101, t102, t103, t104, t105;
	FPvalue t106, t107, t108, t109, t110, t111, t112, t113, t114, t115, t116, t117;
	FPvalue t118, t119, t120, t121, t122, t123, t124, t125, t126, t127, t129, t130;
	FPvalue t131, t134, t135, t136, t137, t138, t139, t140, t141, t142, t143, t144;
	FPvalue t146, t147, t148, t150, t152, t153, t154, t155, t156, t157, t158, t159;
	FPvalue t160, t162, t163, t164, t165, t166, t167, t168, t169, t171, t173, t175;
	FPvalue t179, t181, t182, t185, t186, t187, t189, t190, t195, t196, t197, t198;
	FPvalue t199, t200, t201, t202, t203, t204, t205, t206, t207, t208, t210, t211;
	FPvalue t212, t213, t214, t219, t221, t223, t225, t226, t228, t229, t232, t233;
	FPvalue t235, t236, t237, t238, t239, t240, t241, t242, t243, t245, t249, t255;
	FPvalue t257, t259, t261, t265, t266, t272, t273, t276, t278, t280, t282, t284;
	FPvalue t285, t287, t289, t292, t294, t306, t310, t311, t319, t321, t325, t326;
	FPvalue t327, t329, t330, t331, t336, t337, t342, t344, t345, t348, t349, t353;
	FPvalue t357, t364, t367, t370, t371, t372, t373, t374, t375, t376, t377, t386;
	FPvalue t389, t393, t404, t411, t413, t415, t417, t419, t420, t432, t436, t441;
	FPvalue t457, t474, t482, t498, t504, t513, t530;
	const FPvalue two = (FPvalue) 2.0;
	const FPvalue three = (FPvalue) 3.0;

	t1 = P[0][2]*P[2][0];
	t2 = P[1][1]*P[3][0];
	t3 = t1*t2;
	t4 = P[0][1]*P[2][0];
	t5 = P[3][2]*P[1][0];
	t6 = t4*t5;
	t7 = P[1][2]*P[4][0];
	t9 = P[1][2]*P[3][0];
	t10 = t4*t9;
	t11 = P[4][1]*P[3][0];
	t12 = t1*t11;
	t13 = P[3][1]*P[4][0];
	t15 = P[3][1]*P[1][0];
	t16 = t1*t15;
	t17 = P[4][1]*P[1][0];
	t18 = t1*t17;
	t19 = P[1][1]*P[4][0];
	t21 = P[4][1]*P[2][0];
	t22 = t21*t9;
	t25 = P[3][2]*P[4][0];
	t28 = P[4][2]*P[3][0];
	t30 = two*t4*t28;
	t31 = P[3][1]*P[2][0];
	t34 = P[4][2]*P[1][0];
	t36 = two*t4*t34;
	t37 = P[0][0]*P[3][1];
	t38 = P[2][2]*P[1][0];
	t39 = t37*t38;
	t40 = P[0][0]*P[2][0];
	t41 = P[3][1]*P[1][2];
	t42 = t40*t41;
	t43 = P[0][0]*P[4][1];
	t44 = t43*t9;
	t45 = P[2][2]*P[4][0];
	t46 = t37*t45;
	t47 = P[1][0]*P[2][0];
	t48 = P[4][1]*P[3][2];
	t50 = two*t47*t48;
	t51 = P[3][1]*P[4][2];
	t53 = two*t47*t51;
	t54 = P[0][0]*P[2][1];
	t55 = t54*t28;
	t56 = two*t4*t25-t30+two*t31*t7+t36-t39-t42+t44+t46+t50-t53+t55;
	t58 = t54*t7;
	t59 = t37*t34;
	t60 = t54*t5;
	t61 = P[1][1]*P[2][0];
	t65 = two*t61*t28;
	t66 = t54*t25;
	t67 = t43*t5;
	t68 = P[0][0]*P[1][1];
	t69 = P[2][2]*P[3][0];
	t70 = t68*t69;
	t71 = t68*t25;
	t72 = t68*t28;
	t74 = t40*t51;
	t75 = P[0][0]*P[2][2];
	t76 = t75*t19;
	t77 = t40*t48;
	t78 = P[3][2]*P[1][1];
	t79 = t40*t78;
	t80 = P[1][2]*P[2][0];
	t81 = t43*t80;
	t82 = t75*t17;
	t83 = P[0][0]*P[4][2];
	t84 = t83*t61;
	t85 = t37*t7;
	t86 = t43*t69;
	t87 = t54*t9;
	t88 = P[2][1]*P[1][0];
	t89 = t83*t88;
	t90 = t74-t76-t77+t79+t81+t82-t84-t85-t86-t87-t89;
	t93 = P[4][2]*P[1][1];
	t94 = t93*P[2][0];
	t95 = t61*P[3][2];
	t96 = P[4][1]*P[1][2];
	t97 = t96*P[2][0];
	t98 = t31*P[1][2];
	t99 = t21*P[3][2];
	t100 = t31*P[4][2];
	t101 = P[2][2]*P[1][1];
	t102 = t101*P[4][0];
	t103 = t93*P[3][0];
	t104 = t101*P[3][0];
	t105 = t78*P[4][0];
	t106 = P[2][1]*P[4][0];
	t107 = t106*P[3][2];
	t108 = t17*P[3][2];
	t109 = -t94+t95+t97-t98-t99+t100+t102+t103-t104-t105+t107+t108;
	t110 = P[2][1]*P[3][0];
	t111 = t110*P[1][2];
	t112 = P[2][1]*P[4][2];
	t113 = t112*P[1][0];
	t114 = t41*P[4][0];
	t115 = P[2][2]*P[4][1];
	t116 = t115*P[3][0];
	t117 = t115*P[1][0];
	t118 = t11*P[1][2];
	t119 = P[2][2]*P[3][1];
	t120 = t119*P[4][0];
	t121 = t119*P[1][0];
	t122 = t88*P[3][2];
	t123 = t51*P[1][0];
	t124 = t110*P[4][2];
	t125 = P[2][1]*P[1][2];
	t126 = t125*P[4][0];
	t127 = t111+t113+t114+t116-t117-t118-t120+t121-t122-t123-t124-t126;
	t129 = 1/(t109+t127);
	M[0][0] = (-two*t3-two*t6-two*t4*t7+two*t10+two*t12-two*t1*t13+two*t16
		-two*t18+two*t1*t19-two*t22+t56+t58+t59+t60-two*t61*t25+t65-t66-t67+t70+t71-t72
		+t90)*t129;
	t130 = P[0][1]*P[3][1];
	t131 = t130*t34;
	t134 = P[0][1]*P[2][1];
	t135 = t134*t5;
	t136 = t134*t28;
	t137 = P[0][1]*P[4][2];
	t138 = t137*t88;
	t139 = t130*t7;
	t140 = P[0][1]*P[4][1];
	t141 = t140*t5;
	t142 = t134*t9;
	t143 = t130*t45;
	t144 = t130*t38;
	t146 = t140*t9;
	t147 = t134*t7;
	t148 = t140*t69;
	t150 = two*t106*t41;
	t152 = two*t54*t93;
	t153 = t4*t48;
	t154 = t137*t61;
	t155 = t4*t41;
	t156 = P[0][1]*P[1][1];
	t157 = t156*t25;
	t158 = t4*t78;
	t159 = t140*t80;
	t160 = -t146+t147+t148-t150+t152-t153-t154-t155-t157+t158+t159;
	t162 = t156*t28;
	t163 = t156*t69;
	t164 = t4*t51;
	t165 = P[0][1]*P[2][2];
	t166 = t165*t19;
	t167 = t165*t17;
	t168 = t134*t25;
	t169 = P[0][2]*P[2][1];
	t171 = two*t169*t15;
	t173 = two*t169*t19;
	t175 = two*t88*t51;
	t179 = t169*t13;
	t181 = t169*t2;
	t182 = t106*t78;
	t185 = t110*t93;
	t186 = t54*t51;
	t187 = t54*t78;
	t189 = t54*t41;
	t190 = t179-t169*t11+t181+t182+t110*t96+t54*t48-t185-t186-t187-t54*t96+t189;
	M[0][1] = -(-t131-two*t88*t48+t135+t136-t138+t139+t141-t142-t143+t144+
		t160+t162-t163+t164+t166-t167-t168-t171-t173+t175+two*t169*t17+two*t190)*t129;
	t195 = P[0][2]*P[3][1];
	t196 = t195*t38;
	t197 = t195*t34;
	t198 = t169*t5;
	t199 = P[0][2]*P[4][1];
	t200 = t199*t9;
	t201 = t195*t45;
	t202 = P[0][2]*P[2][2];
	t203 = t202*t17;
	t204 = t169*t25;
	t205 = P[0][2]*P[1][1];
	t206 = t205*t25;
	t207 = t205*t28;
	t208 = t205*t69;
	t210 = t202*t19;
	t211 = t1*t48;
	t212 = t1*t51;
	t213 = t169*t28;
	t214 = t169*t7;
	t219 = P[0][0]*P[3][2];
	t221 = two*t219*t101;
	t223 = two*t5*t115;
	t225 = two*t25*t101;
	t226 = P[0][1]*P[3][2];
	t228 = two*t226*t45;
	t229 = -t210-t211+t212-t213-t214-two*t165*t34+two*t165*t28-t221-t223+t225-
		t228;
	t232 = two*t219*t115;
	t233 = t1*t41;
	t235 = two*t226*t38;
	t236 = t199*t5;
	t237 = t169*t9;
	t238 = t1*t78;
	t239 = t199*t80;
	t240 = t199*t69;
	t241 = P[0][2]*P[4][2];
	t242 = t241*t61;
	t243 = t241*t88;
	t245 = t195*t7;
	t249 = two*t75*t41;
	t255 = two*t45*t41;
	t257 = two*t165*t9;
	t259 = two*t75*t96;
	t261 = two*t69*t96;
	t265 = two*t165*t7;
	t266 = t245-two*t75*t51+t249+two*t38*t51+two*t75*t93-t255-t257-t259+t261
		-two*t69*t93+t265;
	M[0][2] = -(-t196-t197-t198-t200+t201+t203+t204-t206+t207+t208+t229+t232-
		t233+t235+t236+t237+t238+t239-t240-t242+t243+t266)*t129;
	t272 = two*t199*P[3][0];
	t273 = -t122+t121-t120+t118-t117+t116-t114+t113+t111-t108-t272;
	t276 = two*t226*P[4][0];
	t278 = two*t219*P[4][1];
	t280 = two*t199*P[1][0];
	t282 = two*t205*P[4][0];
	t284 = two*t137*P[1][0];
	t285 = P[0][1]*P[1][2];
	t287 = two*t285*P[4][0];
	t289 = two*t43*P[1][2];
	t292 = two*t83*P[1][1];
	t294 = two*t137*P[3][0];
	t306 = two*t195*P[4][0];
	t310 = two*t37*P[4][2];
	t311 = t292+t294-two*t285*P[3][0]-two*t195*P[1][0]+two*t226*P[1][0]+two*t205*P[3][0]-t94-two*
		t219*P[1][1]+t306+two*t37*P[1][2]-t310;
	M[0][3] = -(t107+t105-t104-t103+t102+t100-t99-t126-t124+t123+t273-t276+
		t278+t280-t282+t95-t284-t98+t97+t287-t289+t311)*t129;
	t319 = t55+t58-t59-t60-t66+t67+t70+t71-t72-t74-t76;
	t321 = P[0][1]*P[1][0];
	t325 = P[0][2]*P[1][0];
	t326 = t325*t11;
	t327 = t321*t69;
	t329 = t321*t28;
	t330 = t17*t69;
	t331 = t88*t28;
	t336 = t325*t110;
	t337 = -t326-t327-t15*t45+t329+t330-t331+t325*t13+t321*t45+t88*t25-t325*
		t106+t336;
	M[1][0] = (two*t6-two*t16+two*t18-t36+t39+t42+t44+t46-t50+t53+t319+t77-
		t79-t81-t82+t84-t85-t86-t87+t89-two*t321*t25+two*t337)*t129;
	t342 = t205*t31;
	t344 = t205*t13;
	t345 = t19*t119;
	t348 = t61*t51;
	t349 = t68*t51;
	t353 = t68*t119;
	t357 = two*t353+two*t68*t48+t131+t135+t136-t138-t139-t141-t142+t143-t144;
	t364 = t162-t163-t164+t166+t167-t168-t173+two*t181+two*t182-two*t185-two*
		t187;
	M[1][1] = (two*t205*t21-two*t342-two*t205*t11+two*t344-two*t345+two*t2*
		t115-two*t61*t48+two*t348-two*t349-two*t68*t115+t357+t146+t147-t148+t152+t153-
		t154+t155-t157+t158-t159+t364)*t129;
	t367 = P[4][2]*P[2][0];
	t370 = t226*t7;
	t371 = t25*t125;
	t372 = t226*t80;
	t373 = P[2][0]*P[3][2];
	t374 = t373*t96;
	t375 = t219*t125;
	t376 = t219*t96;
	t377 = P[0][0]*P[1][2];
	t386 = two*t80*t51-two*t9*t112-t196+t197+t198-t200+t201+t203-t204+t206-t207
		;
	t389 = t239-t240+t242-t243+t245+t249-t255-t257-t259+t261+t265;
	M[1][2] = (-two*t285*t367+two*t285*t28-two*t370+two*t371+two*t372-two*
		t374-two*t375+two*t376-two*t377*t51+two*t377*t112+t386+t208-t210+t211-t212+t213
		-t214-t233-t236+t237-t238+t389)*t129;
	t393 = t122-t121-t120+t118+t117+t116-t114-t113-t111-t108-t272;
	t404 = two*t169*P[4][0];
	t411 = two*t165*P[4][0];
	t413 = two*t54*P[4][2];
	t415 = two*t75*P[4][1];
	t417 = two*t137*P[2][0];
	t419 = two*t199*P[2][0];
	t420 = t411+t98-t97+t413-t415-t417+t419+t294+t94+t306-t310;
	M[1][3] = (t107+t105+t104-t103-t102+t100-t99+t126-t124+t123+t393-t276+
		t278-t95+two*t169*P[3][0]-two*t1*P[3][1]-two*t165*P[3][0]+two*t37*P[2][2]-t404+two*t226*P[2][0]-two*t219
		*P[2][1]+t420)*t129;
	t432 = t58+t59+t60+t65-t66-t67+three*t70+t71-three*t72-t74-t76;
	t436 = P[0][2]*P[3][0];
	t441 = P[0][1]*P[3][0];
	t457 = -t85-three*t86-three*t87-t89-two*t69*t19-two*t326-two*t327+two*t329+two*
		t330-two*t331+two*t336;
	M[2][0] = (-two*t3+two*t10+two*t12-two*t22-t30-t39+t42+three*t44+t46+three*
		t55+t432+t77-t79-t81+two*t110*t7+two*t436*t19-two*t436*t106-two*t441*t7+two*
		t441*t45+t82+t84+t457)*t129;
	t474 = -two*t31*t96+two*t15*t115+three*t131+t135+t136-t138-three*t139-t141-t142
		+three*t143-three*t144;
	t482 = -t162+t163-three*t164-t166+t167-t168+t171-t175-two*t179+two*t186-two*
		t189;
	M[2][1] = (-two*t342+two*t344-two*t345+two*t348-two*t349+two*t353-two*t37
		*t115+two*t37*t96+two*t195*t21-two*t195*t17+t474+t146+t147-t148+t150+t153+t154+
		three*t155+t157-t158-t159+t482)*t129;
	t498 = two*t219*t93-two*t226*t34+t196-t197-three*t198-t200-t201-t203+three*t204
		-three*t206+t207;
	t504 = t232-t233+t235+three*t236+t237+three*t238+t239+t240-t242+t243+t245;
	M[2][2] = -(two*t370-two*t371-two*t372+two*t374+two*t375-two*t376+two*
		t226*t367+two*t5*t112-two*t373*t93-two*t219*t112+t498-t208+t210-three*t211+t212-
		t213-t214-t221-t223+t225-t228+t504)*t129;
	t513 = -t122+t121-t120-t118-three*t117+t116+t114+three*t113+t111+t108+t280;
	t530 = -t419+t287-t289+t292-three*t94+two*t54*P[1][2]-two*t68*P[2][2]-two*t4*P[1][2]+two*t1*
		P[1][1]-two*t325*P[2][1]+two*t321*P[2][2];
	M[2][3] = -(t107-t105-t104+t103+three*t102+t100-t99-three*t126-t124-t123+t513
		-t282+t95-t284+t404-t411-t98+three*t97-t413+t415+t417+t530)*t129;
	M[3][0] = P[0][0];
	M[3][1] = P[0][1];
	M[3][2] = P[0][2];
	M[3][3] = 1.0;
}


/** A few simple double-precision analytical 3D geometry routines. ***/

double vlengthd(const double *v)
{
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void vscaled(double *v, double div)
{
	v[0] *= div;
	v[1] *= div;
	v[2] *= div;
}

void vnormald(double *v)
{
	vscaled(v,1.0/vlengthd(v));
}

/* GRAPHICS GEMS I, "Useful 3D Geometry", p297 */
void nearestPointOnPlaneToPoint(double J[4], double P[3], double Q[3])
{
	double JdotP = P[0]*J[0]+P[1]*J[1]+P[2]*J[2];
	double JdotJ = J[0]*J[0]+J[1]*J[1]+J[2]*J[2];
	double k = (J[3] + JdotP)/JdotJ;

	Q[0] = P[0] - k * J[0];
	Q[1] = P[1] - k * J[1];
	Q[2] = P[2] - k * J[2];
}

/* GRAPHICS GEMS I, "Useful 3D Geometry", p299 */
void pointAtLineAndPlaneIntersection(double J[4], double U[3], double V[3], double P[3])
{
	double t = - (J[3] + (U[0]*J[0]+U[1]*J[1]+U[2]*J[2]))/
		(V[0]*J[0]+V[1]*J[1]+V[2]*J[2]);

	P[0] = U[0] + V[0] * t;
	P[1] = U[1] + V[1] * t;
	P[2] = U[2] + V[2] * t;
}

/* Find the plane equation given 3 points. */
void findPlane(double plane[4], double v0[3], double v1[3], double v2[3])
{
	double vec0[3], vec1[3];

	/* Need 2 vectors to find cross product. */
	vec0[0] = v1[0] - v0[0];
	vec0[1] = v1[1] - v0[1];
	vec0[2] = v1[2] - v0[2];

	vec1[0] = v2[0] - v0[0];
	vec1[1] = v2[1] - v0[1];
	vec1[2] = v2[2] - v0[2];

	/* find cross product to get A, B, and C of plane equation */
	plane[0] = vec0[1] * vec1[2] - vec0[2] * vec1[1];
	plane[1] = -(vec0[0] * vec1[2] - vec0[2] * vec1[0]);
	plane[2] = vec0[0] * vec1[1] - vec0[1] * vec1[0];
	plane[3] = -(plane[0] * v0[0] + plane[1] * v0[1] + plane[2] * v0[2]);
}


/* Compute a 4x4 matrix suitable for glMultMatrixd for projecting
into a plane. */
void computeShadowVolume(
						 double L[3],  /* IN: light location */
						 double O[3],  /* IN: cut-out plane */
						 double M[3],  /* IN: cut-out X axis direction */
						 double N[3],  /* IN: cut-out Z axis direction */
						 double G[4],  /* IN: ground plane */
						 double P[5][3])  /* OUT: five mutually non-coplanar points
										  defining the shadow volume projection */
{
	double LO[3];  /* Normalized direction vector from origin to light. */
	double EP[3], LEP[3];

	P[0][0] = O[0];
	P[0][1] = O[1];
	P[0][2] = O[2];

	P[1][0] = O[0] + N[0];
	P[1][1] = O[1] + N[1];
	P[1][2] = O[2] + N[2];

	P[2][0] = O[0] + M[0];
	P[2][1] = O[1] + M[1];
	P[2][2] = O[2] + M[2];

	LO[0] = L[0] - O[0];
	LO[1] = L[1] - O[1];
	LO[2] = L[2] - O[2];
	vnormald(LO);

	pointAtLineAndPlaneIntersection(Pbg, O, LO, P[3]);

	EP[0] = O[0] - N[0] - M[0];
	EP[1] = O[1] - N[1] - M[1];
	EP[2] = O[2] - N[2] - M[2];

	LEP[0] = L[0] - EP[0];
	LEP[1] = L[1] - EP[1];
	LEP[2] = L[2] - EP[2];
	vnormald(LEP);

	pointAtLineAndPlaneIntersection(Pbg, EP, LEP, P[4]);
}

#endif