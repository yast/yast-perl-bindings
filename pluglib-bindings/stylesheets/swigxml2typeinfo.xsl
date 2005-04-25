<?xml version="1.0" encoding="UTF-8"?>
<!-- 
  Description: Convert swig xml wraper to $TYPEINFO in perl
  Author: Martin Lazar <mlazar@suse.cz>
	  Martin Vidner <mvidner@suse.cz>
  Version: 0.2
  Usage: 
    $ swig -xml example.i
    $ sabcmd swigxml2typeinfo.xsl example_wrap.xml examle_typeinfo.pm
-->

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="text"/>
<xsl:strip-space elements="*"/>

<!-- disable copying of text and attrs -->
<xsl:template match="text()|@*"/>

<xsl:template match="cdecl/attributelist/parmlist/parm/attributelist">
    <xsl:param name="type" select="attribute[@name='type']/@value"/>
    <xsl:value-of select="concat(', &#34;', $type, '&#34;')"/>    
</xsl:template>

<xsl:template match="cdecl/attributelist">
    <xsl:param name="class"/>
    <xsl:param name="name" select="attribute[@name='sym_name']/@value"/>
    <xsl:param name="type" select="attribute[@name='type']/@value"/>
    <xsl:param name="view" select="attribute[@name='view']/@value"/>
    <xsl:choose>
	<xsl:when test="$view='globalfunctionHandler'">
	    <xsl:value-of select="concat('        ', $name,' => [&#34;function&#34;, &#34;', $type, '&#34;')"/>
	    <xsl:apply-templates select="parmlist"/>
	    <xsl:text>],&#10;</xsl:text>
	</xsl:when>
    </xsl:choose>
</xsl:template>

<xsl:template match="cdecl/attributelist" mode="class">
    <xsl:param name="class"/>
    <xsl:param name="name" select="attribute[@name='sym_name']/@value"/>
    <xsl:param name="type" select="attribute[@name='type']/@value"/>
    <xsl:param name="view" select="attribute[@name='view']/@value"/>
    <xsl:choose>
	<xsl:when test="$view='memberfunctionHandler'">
	    <xsl:value-of select="concat('        ', $name,' => [&#34;function&#34;, &#34;', $type, '&#34;, &#34;any&#34;')"/>
	    <xsl:apply-templates select="parmlist"/>
	    <xsl:text>],&#10;</xsl:text>
	</xsl:when>
	<xsl:when test="$view='variableHandler'">
	    <xsl:value-of select="concat('        swig_', $name, '_get => [&#34;function&#34;, &#34;', $type, '&#34;, &#34;any&#34;],&#10;')"/>
	    <xsl:value-of select="concat('        swig_', $name, '_set => [&#34;function&#34;, &#34;void&#34;, &#34;any&#34;, &#34;', $type, '&#34;],&#10;')"/>
	</xsl:when>
    </xsl:choose>
</xsl:template>

<xsl:template match="class" mode="class">
    <xsl:param name="class" select="attributelist/attribute[@name='sym_name']/@value"/>
    <xsl:text>package </xsl:text>
    <xsl:value-of select="/top/attributelist/attribute[@name='module']/@value"/>
    <xsl:text>::</xsl:text>
    <xsl:value-of select="$class"/>
    <xsl:text>;&#10;</xsl:text>
    <xsl:text>BEGIN {&#10;</xsl:text>
    <xsl:text>    %TYPEINFO = (&#10;        ALL_METHODS => 0,&#10;</xsl:text>
    <xsl:text>        new => [&#34;function&#34;, &#34;any&#34;],&#10;</xsl:text>
    <xsl:apply-templates mode="class">
	<xsl:with-param name="class" select="$class"/>
    </xsl:apply-templates>
    <xsl:text>    );&#10;}&#10;&#10;</xsl:text>
    <xsl:text>1;&#10;</xsl:text>
</xsl:template>


<xsl:template match="/">
    <xsl:text>package </xsl:text>
    <xsl:value-of select="/top/attributelist/attribute[@name='module']/@value"/>
    <xsl:text>;&#10;</xsl:text>
    <xsl:text>BEGIN {&#10;</xsl:text>
    <xsl:text>    %TYPEINFO = (&#10;        ALL_METHODS => 0,&#10;</xsl:text>
    <xsl:apply-templates/>
    <xsl:text>    );&#10;}&#10;&#10;</xsl:text>
    <xsl:apply-templates mode="class"/>
</xsl:template>

</xsl:stylesheet>
