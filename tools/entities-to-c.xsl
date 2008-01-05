<?xml version="1.0"?>
<!--
$Id: characters.xsl,v 1.2 2003/09/17 14:51:46 davidc Exp $

mml6.xsl David Carlisle
Generate bycodes.html, byalpha.html and the alphabetic glyph tables
for MathML chapter 6.
XSL sheet takes same parameters as mathmlspec.xsl
takes unicode.xml as input file.
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:saxon="http://icl.com/saxon"  
                extension-element-prefixes="saxon"
                version="1.0">

<xsl:param name="status">WD</xsl:param>
<xsl:param name="css">http://www.w3.org/StyleSheets/TR</xsl:param>
<xsl:param name="icon">http://www.w3.org/Icons/WWW</xsl:param>
<xsl:param name="glyphs">glyphs</xsl:param>

<xsl:output method="text"/>

<xsl:key name="set" match="entity" use="@set"/>

<xsl:template match="charlist">

<xsl:call-template name="byalpha"/>


</xsl:template>





<xsl:template name="alphatable">
<xsl:param name="set"/>
<xsl:variable name="d">
<xsl:choose>
<xsl:when test="starts-with($set,'957')">iso9573-13</xsl:when>
<xsl:when test="starts-with($set,'88')">iso8879</xsl:when>
<xsl:when test="starts-with($set,'mml')">mathml</xsl:when>
<xsl:when test="starts-with($set,'htm')">html</xsl:when>
</xsl:choose>
</xsl:variable>
<xsl:variable name="f">
<xsl:choose>
<xsl:when test="starts-with($set,'957')"><xsl:value-of select="substring-after($set,'13-')"/></xsl:when>
<xsl:when test="starts-with($set,'88')"><xsl:value-of select="substring-after($set,'-')"/></xsl:when>
<xsl:when test="starts-with($set,'html')"><xsl:value-of select="substring-after($set,'-')"/></xsl:when>
<xsl:otherwise><xsl:value-of select="$set"/></xsl:otherwise>
</xsl:choose>
</xsl:variable>
<xsl:variable name="t">
<xsl:value-of select="translate($f,'abcdefghijklmnopqrstuvwxyz',
                           'ABCDEFGHIJKLMNOPQRSTUVWXYZ')"/>
Characters and Glyphs</xsl:variable>

<saxon:output method="html" 
     doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN"
     doctype-system="http://www.w3.org/TR/html4/loose.dtd"
     href="{$d}/{$f}.html">
<html>
<head>
<title><xsl:value-of select="$t"/></title>
  <xsl:choose>
    <xsl:when test="$status='REC' or $status='rec'">
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-REC"></link>
    </xsl:when>
    <xsl:when test="$status='PR' or $status='pr'">
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-PR"></link>
    </xsl:when>
    <xsl:when test="$status='CR' or $status='cr'">
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-CR"></link>
    </xsl:when>
    <xsl:otherwise>
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-WD"></link>
    </xsl:otherwise>
  </xsl:choose>
<style>
.ignore {background-color: #AAAAAA;}
</style>
</head>
<body>
<h1><xsl:value-of select="$t"/></h1>
<p><a href="../overview.html">up</a></p>
<p><a href="../../DTD/mathml2/{$d}/{$f}.ent">entity file</a></p>
<table border="1">
<tr>
<th>Name</th>
<th>Unicode</th>
<th>Glyph</th>
<th>Unicode Name</th>
<th>Description</th>
<th>Aliases</th>
</tr>
<xsl:for-each select="key('set',$set)">
<xsl:sort select="@id"/>
<tr>
    <xsl:if test="../description/@unicode='provisional'">
      <xsl:attribute name="class">provisional</xsl:attribute>
      <xsl:message><xsl:value-of select="concat('Provisional: ',$set,': ',@id)"/></xsl:message>
    </xsl:if>
    <xsl:if test="@optional and @default='IGNORE'">
      <xsl:attribute name="class">ignore</xsl:attribute>
    </xsl:if>
<td>
<xsl:value-of select="@id"/>
</td>
<td>
<xsl:value-of select="substring(../@id,2)"/>
</td>
<td>
<img height="32" 
         width="32"
         src="../{$glyphs}/{substring(../@id,2,3)}/{translate(../@id,'x','')}.png"
         alt="{../@id}"
         >
<xsl:if test="../@image='none'"><xsl:attribute
  name="src">../<xsl:value-of select="$glyphs"/>/none.png</xsl:attribute></xsl:if>
</img>
</td>
<td>
<xsl:value-of select="../description"/>
</td>
<td>
<xsl:value-of select="desc"/>
</td>
<td>
<xsl:if test="not(../entity[@set='mmlalias'] or ../entity[@set=$set][2])">&#160;</xsl:if>
<xsl:for-each select="../entity[@set='mmlalias' or (@set=$set
     and @id!=current()/@id)]/@id">
<xsl:value-of select="."/>
<xsl:if test="position() &lt; last()">, </xsl:if>
</xsl:for-each>
</td>
</tr>
</xsl:for-each>
</table>
</body>
</html>
</saxon:output>
</xsl:template>

<!-- -->

<xsl:template name="letter-like">
<xsl:param name="mathvariant"/>
<xsl:param name="title"/>


<saxon:output method="html" 
     doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN"
     doctype-system="http://www.w3.org/TR/html4/loose.dtd"
     href="{$mathvariant}.html">
<html>
<head>
<title>
  <xsl:value-of select="$title"/> 
</title>
  <style type="text/css">
.error { color: red }
.bmp { color: green ; background-color: yellow }
  </style>
  <xsl:choose>
    <xsl:when test="$status='REC' or $status='rec'">
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-REC"></link>
    </xsl:when>
    <xsl:when test="$status='PR' or $status='pr'">
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-PR"></link>
    </xsl:when>
    <xsl:when test="$status='CR' or $status='cr'">
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-CR"></link>
    </xsl:when>
    <xsl:otherwise>
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-WD"></link>
    </xsl:otherwise>
  </xsl:choose>
</head>
<body>
<h1>
 <xsl:value-of select="$title"/>
</h1>

<p>
<xsl:variable name="x" select=
  "document('')/xsl:stylesheet/xsl:template[@match='charlist']/
   xsl:call-template/xsl:with-param[@select=
 concat(&quot;'&quot;,$mathvariant,&quot;'&quot;)]"/>
<xsl:variable name="n" select=
  "translate($x/following::*[@name='mathvariant'][1]/@select,&quot;&apos;&quot;,'')"/>
<xsl:variable name="p" select=
  "translate($x/preceding::*[@name='mathvariant'][1]/@select,&quot;&apos;&quot;,'')"/>

<a href="overview.html">Chapter 6: Entities, Characters and Fonts</a><br/>
<a href="bycodes.html">All, by codes</a><br/>
<!--
<a href="appendixa.html">Appendix A (DTD)</a><br/>
-->
<a href="byalpha.html">Named MathML Characters, ordered by Name</a><br/>
<xsl:if test="$n">
<a href="{translate($n,'ABCDEFGHIJKLMNOPQRSTUVWXYZ',
         'abcdefghijklmnopqrstuvwxyz')}.html">Next: <xsl:value-of 
select="$n"/></a><br/>
</xsl:if>
<xsl:if test="$p">
<a href="{translate($p,'ABCDEFGHIJKLMNOPQRSTUVWXYZ',
         'abcdefghijklmnopqrstuvwxyz')}.html">Previous: <xsl:value-of 
select="$p"/></a><br/>
</xsl:if>
</p>


<table border="1">
<tr>
<th>Unicode</th>
<th>BMP</th>
<th>Glyph</th>
<th>Unicode Name</th>
<th>MathML Names</th>
</tr>
<xsl:for-each select="character[surrogate/@mathvariant=$mathvariant]">
<xsl:sort select="@id"/>
<xsl:variable name="s" select="substring(surrogate/@ref,2)"/>
<tr>
<xsl:if test="bmp">
  <xsl:attribute name="class">bmp</xsl:attribute>
</xsl:if>
<xsl:for-each select="self::node()[not(bmp)]|id(bmp/@ref)">
<td>
<xsl:value-of select="substring(@id,2)"/>
</td>
<td>
<xsl:value-of select="$s"/>
</td>
<td>
<img height="32" 
         width="32"
         src="{$glyphs}/{substring(@id,2,3)}/{translate(@id,'x','')}.png"
         alt="{@id}"
         >
<xsl:if test="@image='none'"><xsl:attribute
  name="src"><xsl:value-of select="$glyphs"/>/none.png</xsl:attribute></xsl:if>
</img>
</td>
<td>
<xsl:value-of select="description"/>
</td>
<td>
<xsl:variable name="x" select="entity[
    @set='9573-13-isoamsa'
 or @set='9573-13-isoamsb'
 or @set='9573-13-isoamsc'
 or @set='9573-13-isoamsn'
 or @set='9573-13-isoamso'
 or @set='9573-13-isoamsr'
 or @set='9573-13-isogrk3'
 or @set='9573-13-isomfrk'
 or @set='9573-13-isomopf'
 or @set='9573-13-isomscr'
 or @set='9573-13-isotech'
 or @set='8879-isobox'
 or @set='8879-isocyr1'
 or @set='8879-isocyr2'
 or @set='8879-isodia'
 or @set='8879-isolat1'
 or @set='8879-isolat2'
 or @set='8879-isonum'
 or @set='8879-isopub'
 or @set='Xmmlalias'
 or @set='mmlextra']/@id"/>
<xsl:choose>
<xsl:when test="not($x)">&#160;</xsl:when>
<xsl:otherwise>
<xsl:for-each select="$x">
<xsl:value-of select="."/>
<xsl:if test="position() &lt; last()">, </xsl:if>
</xsl:for-each>
</xsl:otherwise>
</xsl:choose>
</td>
</xsl:for-each>
</tr>
</xsl:for-each>
</table>
</body>
</html>
</saxon:output>
</xsl:template>


<!-- -->

<xsl:template name="code-chart">
 <xsl:param name="range"/>
<saxon:output method="html" 
     doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN"
     doctype-system="http://www.w3.org/TR/html4/loose.dtd"
     href="{$range}.html">
<html>
<head>
<title>Unicode Characters:
<xsl:value-of select="concat($range,'00 to ',$range,'FF')"/>
</title>
<style type="text/css">
.unassigned { background-color: red }
.undescribed { background-color: yellow }
.unnamed-mathml { background-color: fuschia }
</style>
  <xsl:choose>
    <xsl:when test="$status='REC' or $status='rec'">
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-REC"></link>
    </xsl:when>
    <xsl:when test="$status='PR' or $status='pr'">
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-PR"></link>
    </xsl:when>
    <xsl:when test="$status='CR' or $status='cr'">
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-CR"></link>
    </xsl:when>
    <xsl:otherwise>
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-WD"></link>
    </xsl:otherwise>
  </xsl:choose>
</head>
<body>
<h1>Unicode Characters:
<xsl:value-of select="concat($range,'00 to ',$range,'FF')"/>
</h1>
<xsl:variable name="x" select=
  "document('')/xsl:stylesheet/xsl:template[@match='charlist']/
   xsl:call-template/xsl:with-param[contains(@select,$range)]"/>
<xsl:variable name="n" select=
  "translate($x/following::*[@name='range'][1]/@select,&quot;&apos;&quot;,'')"/>
<xsl:variable name="p" select=
  "translate($x/preceding::*[@name='range'][1]/@select,&quot;&apos;&quot;,'')"/>

<p>
<a href="overview.html">Chapter 6: Entities, Characters and Fonts</a><br/>
<a href="bycodes.html">All, by codes</a><br/>
<a href="byalpha.html">Named MathML Characters, ordered by Name</a><br/>
<xsl:if test="$n">
<a href="{$n}.html">Next: <xsl:value-of 
select="$n"/></a><br/>
</xsl:if>
<xsl:if test="$p">
<a href="{$p}.html">Previous: <xsl:value-of 
select="$p"/></a><br/>
</xsl:if>


</p>


<table border="1">
<tr>
<th>&#160;</th>
<th><xsl:value-of select="$range"/>0</th>
<th><xsl:value-of select="$range"/>1</th>
<th><xsl:value-of select="$range"/>2</th>
<th><xsl:value-of select="$range"/>3</th>
<th><xsl:value-of select="$range"/>4</th>
<th><xsl:value-of select="$range"/>5</th>
<th><xsl:value-of select="$range"/>6</th>
<th><xsl:value-of select="$range"/>7</th>
<th><xsl:value-of select="$range"/>8</th>
<th><xsl:value-of select="$range"/>9</th>
<th><xsl:value-of select="$range"/>A</th>
<th><xsl:value-of select="$range"/>B</th>
<th><xsl:value-of select="$range"/>C</th>
<th><xsl:value-of select="$range"/>D</th>
<th><xsl:value-of select="$range"/>E</th>
<th><xsl:value-of select="$range"/>F</th>
</tr>
<xsl:call-template name="table-row">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="row" select="'0'"/>
</xsl:call-template>
<xsl:call-template name="table-row">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="row" select="'1'"/>
</xsl:call-template>
<xsl:call-template name="table-row">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="row" select="'2'"/>
</xsl:call-template>
<xsl:call-template name="table-row">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="row" select="'3'"/>
</xsl:call-template>
<xsl:call-template name="table-row">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="row" select="'4'"/>
</xsl:call-template>
<xsl:call-template name="table-row">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="row" select="'5'"/>
</xsl:call-template>
<xsl:call-template name="table-row">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="row" select="'6'"/>
</xsl:call-template>
<xsl:call-template name="table-row">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="row" select="'7'"/>
</xsl:call-template>
<xsl:call-template name="table-row">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="row" select="'8'"/>
</xsl:call-template>
<xsl:call-template name="table-row">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="row" select="'9'"/>
</xsl:call-template>
<xsl:call-template name="table-row">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="row" select="'A'"/>
</xsl:call-template>
<xsl:call-template name="table-row">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="row" select="'B'"/>
</xsl:call-template>
<xsl:call-template name="table-row">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="row" select="'C'"/>
</xsl:call-template>
<xsl:call-template name="table-row">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="row" select="'D'"/>
</xsl:call-template>
<xsl:call-template name="table-row">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="row" select="'E'"/>
</xsl:call-template>
<xsl:call-template name="table-row">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="row" select="'F'"/>
</xsl:call-template>
<tr>
<th>&#160;</th>
<th><xsl:value-of select="$range"/>0</th>
<th><xsl:value-of select="$range"/>1</th>
<th><xsl:value-of select="$range"/>2</th>
<th><xsl:value-of select="$range"/>3</th>
<th><xsl:value-of select="$range"/>4</th>
<th><xsl:value-of select="$range"/>5</th>
<th><xsl:value-of select="$range"/>6</th>
<th><xsl:value-of select="$range"/>7</th>
<th><xsl:value-of select="$range"/>8</th>
<th><xsl:value-of select="$range"/>9</th>
<th><xsl:value-of select="$range"/>A</th>
<th><xsl:value-of select="$range"/>B</th>
<th><xsl:value-of select="$range"/>C</th>
<th><xsl:value-of select="$range"/>D</th>
<th><xsl:value-of select="$range"/>E</th>
<th><xsl:value-of select="$range"/>F</th>
</tr>
</table>

<p>
Key:
</p>

<table border="1">
<tr><th>Cell Style</th><th>Status</th></tr>
<tr><td>&#160;&#160;&#160;</td><td>Unicode Character (Unicode 3.2) with MathML name</td></tr>
<tr><td class="unassigned">&#160;</td><td>Unicode or XML Non-Character</td></tr>
<tr><td class="unnamed-mathml">&#160;</td><td>Character described in
these tables, but currently not given a MathML name</td></tr>
<tr><td><img src="{$glyphs}/none.png" alt="none"/></td><td>Character for which an image is not currently available</td></tr>
</table>
</body>
</html>
</saxon:output>
</xsl:template>

<xsl:template name="table-row">
<xsl:param name="range"/>
<xsl:param name="row"/>
<tr>
<th><xsl:value-of select="$row"/></th>
<xsl:call-template name="table-cell">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="h" select="concat('0',$row)"/>
</xsl:call-template>
<xsl:call-template name="table-cell">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="h" select="concat('1',$row)"/>
</xsl:call-template>
<xsl:call-template name="table-cell">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="h" select="concat('2',$row)"/>
</xsl:call-template>
<xsl:call-template name="table-cell">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="h" select="concat('3',$row)"/>
</xsl:call-template>
<xsl:call-template name="table-cell">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="h" select="concat('4',$row)"/>
</xsl:call-template>
<xsl:call-template name="table-cell">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="h" select="concat('5',$row)"/>
</xsl:call-template>
<xsl:call-template name="table-cell">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="h" select="concat('6',$row)"/>
</xsl:call-template>
<xsl:call-template name="table-cell">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="h" select="concat('7',$row)"/>
</xsl:call-template>
<xsl:call-template name="table-cell">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="h" select="concat('8',$row)"/>
</xsl:call-template>
<xsl:call-template name="table-cell">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="h" select="concat('9',$row)"/>
</xsl:call-template>
<xsl:call-template name="table-cell">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="h" select="concat('A',$row)"/>
</xsl:call-template>
<xsl:call-template name="table-cell">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="h" select="concat('B',$row)"/>
</xsl:call-template>
<xsl:call-template name="table-cell">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="h" select="concat('C',$row)"/>
</xsl:call-template>
<xsl:call-template name="table-cell">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="h" select="concat('D',$row)"/>
</xsl:call-template>
<xsl:call-template name="table-cell">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="h" select="concat('E',$row)"/>
</xsl:call-template>
<xsl:call-template name="table-cell">
 <xsl:with-param name="range" select="$range"/>
 <xsl:with-param name="h" select="concat('F',$row)"/>
</xsl:call-template>
<th><xsl:value-of select="$row"/></th>
</tr>
</xsl:template>


<xsl:template name="table-cell">
<xsl:param name="range"/>
<xsl:param name="h"/>
<td width="40" align="center">
<xsl:variable name="x" select="id(concat('U',$range,$h))"/>
<xsl:choose>
<xsl:when test="$x and not($x/bmp) and not($x/description/@unicode='unassigned')">
  <xsl:attribute name="title">
    <xsl:value-of select="$x/description"/>
  </xsl:attribute>
  <xsl:choose>
  <xsl:when test="$x/entity[
      @set='9573-13-isoamsa'
   or @set='9573-13-isoamsb'
   or @set='9573-13-isoamsc'
   or @set='9573-13-isoamsn'
   or @set='9573-13-isoamso'
   or @set='9573-13-isoamsr'
   or @set='9573-13-isogrk3'
   or @set='9573-13-isomfrk'
   or @set='9573-13-isomopf'
   or @set='9573-13-isomscr'
   or @set='9573-13-isotech'
   or @set='8879-isobox'
   or @set='8879-isocyr1'
   or @set='8879-isocyr2'
   or @set='8879-isodia'
   or @set='8879-isolat1'
   or @set='8879-isolat2'
   or @set='8879-isonum'
   or @set='8879-isopub'
   or @set='Xmmlalias'
   or @set='mmlextra']">
    <xsl:if test="$x/description/@unicode='provisional'">
      <xsl:attribute name="class">provisional</xsl:attribute>
    </xsl:if>
  </xsl:when>
  <xsl:otherwise><!-- not named -->
    <xsl:choose>
      <xsl:when test="$x/description/@unicode='provisional'">
        <xsl:attribute name="class">unnamed-provisional</xsl:attribute>
      </xsl:when>
      <xsl:when test="$x/description/@unicode='unasigned'">
        <xsl:attribute name="class">unassigned</xsl:attribute>
      </xsl:when>
      <xsl:otherwise>
        <xsl:attribute name="class">unnamed-mathml</xsl:attribute>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:otherwise>
  </xsl:choose>
  <xsl:choose>
  <xsl:when test="($x/surrogate and not($x/bmp)) or $x/entity[
      @set='9573-13-isoamsa'
   or @set='9573-13-isoamsb'
   or @set='9573-13-isoamsc'
   or @set='9573-13-isoamsn'
   or @set='9573-13-isoamso'
   or @set='9573-13-isoamsr'
   or @set='9573-13-isogrk3'
   or @set='9573-13-isomfrk'
   or @set='9573-13-isomopf'
   or @set='9573-13-isomscr'
   or @set='9573-13-isotech'
   or @set='8879-isobox'
   or @set='8879-isocyr1'
   or @set='8879-isocyr2'
   or @set='8879-isodia'
   or @set='8879-isolat1'
   or @set='8879-isolat2'
   or @set='8879-isonum'
   or @set='8879-isopub'
   or @set='Xmmlalias'
   or @set='mmlextra']">
  <a href="bycodes.html#{$x/@id}">
  <img height="32" width="32" src="{$glyphs}/{$range}/U{$range}{$h}.png"
    alt="{$x/description}">
<xsl:if test="$x/@image='none'"><xsl:attribute
  name="src"><xsl:value-of select="$glyphs"/>/none.png</xsl:attribute></xsl:if>
</img>
  </a>
  </xsl:when>
  <xsl:otherwise>
  <img height="32" width="32" src="{$glyphs}/{$range}/U{$range}{$h}.png"
    alt="{$x/description}">
<xsl:if test="$x/@image='none'"><xsl:attribute
  name="src"><xsl:value-of select="$glyphs"/>/none.png</xsl:attribute></xsl:if>
</img>
  </xsl:otherwise>
  </xsl:choose>
</xsl:when>
<xsl:when test="$x/bmp or $x/description/@unicode='unassigned'">
  <xsl:attribute name="class">unassigned</xsl:attribute>
   <xsl:text>&#160;</xsl:text>
</xsl:when>
<xsl:when test="$range='000' and (starts-with($h,'0') or starts-with($h,'1'))">
  <xsl:attribute name="class">unassigned</xsl:attribute>
   <xsl:text>&#160;</xsl:text>
</xsl:when>
<xsl:otherwise>
  <xsl:attribute name="class">undescribed</xsl:attribute>
   <xsl:text>&#160;</xsl:text>
</xsl:otherwise>
</xsl:choose>
</td>
</xsl:template>


<!-- -->
<xsl:template name="bycodes">
<saxon:output method="html" 
     indent="no"
     doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN"
     doctype-system="http://www.w3.org/TR/html4/loose.dtd"
     href="bycodes.html">
<html>
<head>
<title>
  Characters Ordered by Unicode
</title>
<xsl:text>&#10;</xsl:text>
  <style type="text/css">.error { color: red }</style>
  <xsl:choose>
    <xsl:when test="$status='REC' or $status='rec'">
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-REC"></link>
    </xsl:when>
    <xsl:when test="$status='PR' or $status='pr'">
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-PR"></link>
    </xsl:when>
    <xsl:when test="$status='CR' or $status='cr'">
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-CR"></link>
    </xsl:when>
    <xsl:otherwise>
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-WD"></link>
    </xsl:otherwise>
  </xsl:choose>
<xsl:text>&#10;</xsl:text>
</head>
<xsl:text>&#10;</xsl:text>
<body>
<h1>Characters Ordered by Unicode</h1>
<xsl:text>&#10;</xsl:text>
<p>
<a href="overview.html">Chapter 6: Entities, Characters and Fonts</a><br/>
<a href="byalpha.html">Named MathML Characters, ordered by Name</a><br/>
</p>

<pre>
<xsl:text>&#10;</xsl:text>
<xsl:for-each select="character[entity[
    @set='9573-13-isoamsa'
 or @set='9573-13-isoamsb'
 or @set='9573-13-isoamsc'
 or @set='9573-13-isoamsn'
 or @set='9573-13-isoamso'
 or @set='9573-13-isoamsr'
 or @set='9573-13-isogrk3'
 or @set='9573-13-isomfrk'
 or @set='9573-13-isomopf'
 or @set='9573-13-isomscr'
 or @set='9573-13-isotech'
 or @set='8879-isobox'
 or @set='8879-isocyr1'
 or @set='8879-isocyr2'
 or @set='8879-isodia'
 or @set='8879-isolat1'
 or @set='8879-isolat2'
 or @set='8879-isonum'
 or @set='8879-isopub'
 or @set='mmlextra']
or surrogate]">
<xsl:sort select="@id"/>
<a name="{@id}"/>
<xsl:choose>
<xsl:when test="@image='none'"><xsl:value-of select="@id"/></xsl:when>
<xsl:otherwise>
<a href="{$glyphs}/{substring(@id,2,3)}/{translate(@id,'x','')}.png">
<xsl:value-of select="@id"/>
</a>
</xsl:otherwise>
</xsl:choose>
<xsl:text>, </xsl:text>
<xsl:value-of select="description"/>
<xsl:value-of select="substring(
'                                      ',
string-length(description))"/>
<xsl:text>, </xsl:text>
<xsl:for-each select="entity[
    @set='9573-13-isoamsa'
 or @set='9573-13-isoamsb'
 or @set='9573-13-isoamsc'
 or @set='9573-13-isoamsn'
 or @set='9573-13-isoamso'
 or @set='9573-13-isoamsr'
 or @set='9573-13-isogrk3'
 or @set='9573-13-isomfrk'
 or @set='9573-13-isomopf'
 or @set='9573-13-isomscr'
 or @set='9573-13-isotech'
 or @set='8879-isobox'
 or @set='8879-isocyr1'
 or @set='8879-isocyr2'
 or @set='8879-isodia'
 or @set='8879-isolat1'
 or @set='8879-isolat2'
 or @set='8879-isonum'
 or @set='8879-isopub'
 or @set='mmlextra'
 or @set='mmlalias']/@id">
<xsl:value-of select="."/>
<xsl:if test="position() &lt; last()">, </xsl:if>
</xsl:for-each>
<xsl:text>&#10;</xsl:text>
</xsl:for-each>
</pre>
</body>
</html>
</saxon:output>
</xsl:template>

<xsl:template name="byalpha">
<saxon:output method="text" href="entities.c">
	<xsl:text>static const struct {
		const char *name;
		const char *unicode;
		const char *comment;
		} entities[] = {
	</xsl:text>
<xsl:for-each select="character/entity[
    @set='9573-13-isoamsa'
 or @set='9573-13-isoamsb'
 or @set='9573-13-isoamsc'
 or @set='9573-13-isoamsn'
 or @set='9573-13-isoamso'
 or @set='9573-13-isoamsr'
 or @set='9573-13-isogrk3'
 or @set='9573-13-isomfrk'
 or @set='9573-13-isomopf'
 or @set='9573-13-isomscr'
 or @set='9573-13-isotech'
 or @set='8879-isobox'
 or @set='8879-isocyr1'
 or @set='8879-isocyr2'
 or @set='8879-isodia'
 or @set='8879-isolat1'
 or @set='8879-isolat2'
 or @set='8879-isonum'
 or @set='8879-isopub'
 or @set='mmlextra'
 or @set='mmlalias']">
<xsl:sort select="@id"/>
<xsl:text>{"</xsl:text>
<xsl:value-of select="@id"/>
<xsl:text>",</xsl:text>
<xsl:value-of select="substring(
'                        ',
string-length(@id))"/>
<xsl:text>"</xsl:text>
<xsl:text></xsl:text>
<xsl:value-of select="../@id"/>
<xsl:text>", "[</xsl:text>
<xsl:value-of select="normalize-space(../description)"/>
<xsl:text>]"},
</xsl:text>
</xsl:for-each>
</saxon:output>
</xsl:template>

<xsl:template name="variants">
<saxon:output method="html" 
     doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN"
     doctype-system="http://www.w3.org/TR/html4/loose.dtd"
     href="variants.html">
<html>
<head>
<title>Unicode Characters: Variants</title>
  <xsl:choose>
    <xsl:when test="$status='REC' or $status='rec'">
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-REC"></link>
    </xsl:when>
    <xsl:when test="$status='PR' or $status='pr'">
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-PR"></link>
    </xsl:when>
    <xsl:when test="$status='CR' or $status='cr'">
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-CR"></link>
    </xsl:when>
    <xsl:otherwise>
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-WD"></link>
    </xsl:otherwise>
  </xsl:choose>
</head>
<body>
<h1>Unicode Characters: Variants</h1>
<p>
<a href="overview.html">Chapter 6: Entities, Characters and Fonts</a><br/>
<a href="bycodes.html">All, by codes</a><br/>
</p>
<xsl:call-template name="compose-table"/>
</body>
</html>
</saxon:output>
</xsl:template>



<xsl:template name="cancellations">
<saxon:output method="html" 
     doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN"
     doctype-system="http://www.w3.org/TR/html4/loose.dtd"
     href="cancellations.html">
<html>
<head>
<title>Unicode Characters: Cancellations</title>
  <xsl:choose>
    <xsl:when test="$status='REC' or $status='rec'">
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-REC"></link>
    </xsl:when>
    <xsl:when test="$status='PR' or $status='pr'">
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-PR"></link>
    </xsl:when>
    <xsl:when test="$status='CR' or $status='cr'">
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-CR"></link>
    </xsl:when>
    <xsl:otherwise>
    <link rel="stylesheet" type="text/css" href="{$css}/W3C-WD"></link>
    </xsl:otherwise>
  </xsl:choose>
</head>
<body>
<h1>Unicode Characters: Cancellations</h1>
<p>
<a href="overview.html">Chapter 6: Entities, Characters and Fonts</a><br/>
<a href="bycodes.html">All, by codes</a><br/>
</p>

<xsl:call-template name="compose-table">
  <xsl:with-param name="char" select="'-00338'"/>
</xsl:call-template>


<xsl:call-template name="compose-table">
  <xsl:with-param name="char" select="'-020D2'"/>
</xsl:call-template>


<xsl:call-template name="compose-table">
  <xsl:with-param name="char" select="'-020E5'"/>
</xsl:call-template>

<!--
<xsl:call-template name="compose-table">
  <xsl:with-param name="char" select="'-00307'"/>
</xsl:call-template>
-->

</body>
</html>
</saxon:output>
</xsl:template>

<xsl:template name="compose-table">
<xsl:param name="char" select="'-0FE00'"/>
<xsl:variable name="c" select="id(translate($char,'-','U'))"/>
<h2><xsl:value-of select="$c/description"/></h2>
<table border="1">
<tr>
<th>&#160;</th>
<th>Unicode</th>
<th>Glyph</th>
<th>Unicode Name</th>
<th>MathML Names</th>
</tr>
<tr>
<td>&#160;</td>
<td>
<xsl:value-of select="substring($c/@id,2)"/>
</td>
<td>
<img height="32" 
         width="32"
         src="{$glyphs}/{substring($c/@id,2,3)}/{$c/@id}.png"
         alt="{$c/@id}"
         >
<xsl:if test="$c/@image='none'"><xsl:attribute
  name="src"><xsl:value-of select="$glyphs"/>/none.png</xsl:attribute></xsl:if>
</img>
</td>
<td>
<xsl:value-of select="$c/description"/>
</td>
<td>&#160;</td>
</tr>

<xsl:variable name="cz" select="concat($char,'z')"/>
<xsl:for-each select="character[contains(concat(@id,'z'),$cz)]">
<xsl:sort select="@id"/>
<xsl:variable name="id" select="@id"/>
<xsl:variable name="base" select="id(concat(substring-before($id,$char),substring-after($id,$char)))"/>
<tr>
<xsl:if test="not($base) or ($base/description/@unicode and
not($base/description/@unicode='3.1' or
$base/description/@unicode='3.2'))">
<xsl:attribute name="class">provisional</xsl:attribute>
<xsl:message><xsl:value-of select="@id"/></xsl:message>
</xsl:if>
<th>Base</th>
<td>
<xsl:value-of select="substring($base/@id,2)"/>
</td>
<td>
<img height="32" 
         width="32"
         src="{$glyphs}/{substring($base/@id,2,3)}/{translate($base/@id,'x','')}.png"
         alt="{$base/@id}"
         >
<xsl:if test="$base/@image='none'"><xsl:attribute
  name="src"><xsl:value-of select="$glyphs"/>/none.png</xsl:attribute></xsl:if>
</img>
</td>
<td>
<xsl:value-of select="$base/description"/>
</td>
<td>
<xsl:variable name="x" select="$base/entity[
    @set='9573-13-isoamsa'
 or @set='9573-13-isoamsb'
 or @set='9573-13-isoamsc'
 or @set='9573-13-isoamsn'
 or @set='9573-13-isoamso'
 or @set='9573-13-isoamsr'
 or @set='9573-13-isogrk3'
 or @set='9573-13-isomfrk'
 or @set='9573-13-isomopf'
 or @set='9573-13-isomscr'
 or @set='9573-13-isotech'
 or @set='8879-isobox'
 or @set='8879-isocyr1'
 or @set='8879-isocyr2'
 or @set='8879-isodia'
 or @set='8879-isolat1'
 or @set='8879-isolat2'
 or @set='8879-isonum'
 or @set='8879-isopub'
 or @set='Xmmlalias'
 or @set='mmlextra']/@id"/>
<xsl:choose>
<xsl:when test="not($x)">&#160;</xsl:when>
<xsl:otherwise>
<xsl:for-each select="$x">
<xsl:value-of select="."/>
<xsl:if test="position() &lt; last()">, </xsl:if>
</xsl:for-each>
</xsl:otherwise>
</xsl:choose>
</td>
</tr>
<tr>
<xsl:if test="not(description/@unicode='combination' or description/@unicode='3.2')">
<xsl:attribute name="class">provisional</xsl:attribute>
<xsl:message><xsl:value-of select="@id"/></xsl:message>
</xsl:if>
<th>Variant</th>
<td>
<xsl:value-of select="substring($id,2)"/>
</td>
<td>
<img height="32" 
         width="32"
         src="{$glyphs}/{substring($id,2,3)}/{translate($id,'x','')}.png"
         alt="{$id}"
         >
<xsl:if test="@image='none'"><xsl:attribute
  name="src"><xsl:value-of select="$glyphs"/>/none.png</xsl:attribute></xsl:if>
</img>
</td>
<td>
<xsl:value-of select="description"/>
</td>
<td>
<xsl:variable name="x" select="entity[
    @set='9573-13-isoamsa'
 or @set='9573-13-isoamsb'
 or @set='9573-13-isoamsc'
 or @set='9573-13-isoamsn'
 or @set='9573-13-isoamso'
 or @set='9573-13-isoamsr'
 or @set='9573-13-isogrk3'
 or @set='9573-13-isomfrk'
 or @set='9573-13-isomopf'
 or @set='9573-13-isomscr'
 or @set='9573-13-isotech'
 or @set='8879-isobox'
 or @set='8879-isocyr1'
 or @set='8879-isocyr2'
 or @set='8879-isodia'
 or @set='8879-isolat1'
 or @set='8879-isolat2'
 or @set='8879-isonum'
 or @set='8879-isopub'
 or @set='Xmmlalias'
 or @set='mmlextra']/@id"/>
<xsl:choose>
<xsl:when test="not($x)">&#160;</xsl:when><xsl:when test="not($x)">&#160;</xsl:when>
<xsl:otherwise>
<xsl:for-each select="$x">
<xsl:value-of select="."/>
<xsl:if test="position() &lt; last()">, </xsl:if>
</xsl:for-each>
</xsl:otherwise>
</xsl:choose>
</td>
</tr>
</xsl:for-each>
</table>
</xsl:template>


</xsl:stylesheet>
