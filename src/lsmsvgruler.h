#include <lsmsvgstyle.h>

#ifndef LSM_SVG_RULER
#define LSM_SVG_RULER

typedef struct _LsmSvgRuler LsmSvgRuler;

LsmSvgRuler *		lsm_svg_ruler_new 			(double resolution_ppi);
double			lsm_svg_ruler_get_resolution_ppi	(LsmSvgRuler *ruler);

LsmSvgRuler *		lsm_svg_ruler_ref 			(LsmSvgRuler *ruler);
void			lsm_svg_ruler_unref 			(LsmSvgRuler *ruler);
void 			lsm_svg_ruler_push_style 		(LsmSvgRuler *ruler, LsmSvgStyle *style);
void 			lsm_svg_ruler_pop_style 		(LsmSvgRuler *ruler);
LsmSvgStyle *		lsm_svg_ruler_get_style			(LsmSvgRuler *ruler);
gboolean 		lsm_svg_ruler_push_matrix 		(LsmSvgRuler *ruler, const LsmSvgMatrix *matrix);
void	 		lsm_svg_ruler_pop_matrix 		(LsmSvgRuler *ruler);
const LsmSvgMatrix *	lsm_svg_ruler_get_matrix		(LsmSvgRuler *ruler);
void			lsm_svg_ruler_push_viewbox		(LsmSvgRuler *ruler, const LsmBox *viewbox);
void			lsm_svg_ruler_pop_viewbox		(LsmSvgRuler *ruler);
const LsmSvgViewbox *	lsm_svg_ruler_get_viewbox		(LsmSvgRuler *ruler);

double 			lsm_svg_ruler_normalize_length 		(LsmSvgRuler *ruler, const LsmSvgLength *length,
								 LsmSvgLengthDirection direction);
double * 		lsm_svg_ruler_normalize_length_list 	(LsmSvgRuler *ruler, const LsmSvgLengthList *list,
								 LsmSvgLengthDirection direction, unsigned int *n_data);

#endif
