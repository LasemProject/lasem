%module itex2MML 
%{
#include "itex2MML.h"
extern const char * itex2MML_output ();
%}
extern int    itex2MML_filter (const char * buffer, unsigned long length);
extern int    itex2MML_html_filter (const char * buffer, unsigned long length);
extern const char * itex2MML_output ();
