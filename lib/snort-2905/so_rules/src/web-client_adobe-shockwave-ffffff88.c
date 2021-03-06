/* XXX Custom detection.  Below just for generation of the base code. XXX
alert tcp $EXTERNAL_NET $HTTP_PORTS -> $HOME_NET any (msg:"WEB-CLIENT Adobe Shockwave Player Director file FFFFFF88 record integer overflow attempt"; flow:to_client,established; flowbits:isset,http.dir; content:"|88 ff ff ff|"; reference:cve,2010-2876; reference:url,www.adobe.com/support/security/bulletins/apsb10-20.html; classtype:attempted-user; sid:19350;)
*/
/*
 * Vuln Title: XXXX
 *
 * Copyright (C) 2005-2010 Sourcefire, Inc. All Rights Reserved
 *
 * Written by XXXX, Sourcefire VRT <XXXX@sourcefire.com>
 *
 * Auto-generated by XXXX
 *
 * This file may contain proprietary rules that were created, tested and
 * certified by Sourcefire, Inc. (the "VRT Certified Rules") as well as
 * rules that were created by Sourcefire and other third parties and
 * distributed under the GNU General Public License (the "GPL Rules").  The
 * VRT Certified Rules contained in this file are the property of
 * Sourcefire, Inc. Copyright 2005 Sourcefire, Inc. All Rights Reserved.
 * The GPL Rules created by Sourcefire, Inc. are the property of
 * Sourcefire, Inc. Copyright 2002-2005 Sourcefire, Inc. All Rights
 * Reserved.  All other GPL Rules are owned and copyrighted by their
 * respective owners (please see www.snort.org/contributors for a list of
 * owners and their respective copyrights).  In order to determine what
 * rules are VRT Certified Rules or GPL Rules, please refer to the VRT
 * Certified Rules License Agreement.
 */

#include "sf_snort_plugin_api.h"
#include "sf_snort_packet.h"

//#define DEBUG
#ifdef DEBUG
#define DEBUG_SO(code) code
#else
#define DEBUG_SO(code)
#endif

#ifndef READ_LITTLE_32
#define READ_LITTLE_32(p) (*((u_int8_t *)(p) + 3) << 24) \
                | (*((u_int8_t *)(p) + 2) << 16) \
                | (*((u_int8_t *)(p) + 1) << 8)  \
                | (*(p))
#endif

#ifndef READ_LITTLE_16
#define READ_LITTLE_16(p) (*((u_int8_t *)(p) + 1) << 8) \
                | (*(p))
#endif


/* declare detection functions */
int rule19350eval(void *p);

/* declare rule data structures */
/* flow:established, to_client; */
static FlowFlags rule19350flow0 = 
{
    FLOW_ESTABLISHED|FLOW_TO_CLIENT
};

static RuleOption rule19350option0 =
{
    OPTION_TYPE_FLOWFLAGS,
    {
        &rule19350flow0
    }
};
/* flowbits:isset "http.dir"; */
static FlowBitsInfo rule19350flowbits1 =
{
    "http.dir",
    FLOWBIT_ISSET,
    0,
};

static RuleOption rule19350option1 =
{
    OPTION_TYPE_FLOWBIT,
    {
        &rule19350flowbits1
    }
};
// content:"|88 FF FF FF|", depth 0, fast_pattern; 
static ContentInfo rule19350content2 = 
{
    (u_int8_t *) "|88 FF FF FF|", /* pattern (now in snort content format) */
    0, /* depth */
    0, /* offset */
    CONTENT_FAST_PATTERN|CONTENT_BUF_NORMALIZED|CONTENT_RELATIVE, /* flags */
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule19350option2 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule19350content2
    }
};

// content:"Shockwave 3D", distance:-20, within:12, relative
static ContentInfo rule19350content3 =
{
    (u_int8_t *) "Shockwave 3D", /* pattern (now in snort content format) */
    12, /* depth */
    -20, /* offset */
    CONTENT_BUF_NORMALIZED|CONTENT_RELATIVE, /* flags */
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule19350option3 =
{
    OPTION_TYPE_CONTENT,
    {
        &rule19350content3
    }
};

/* references for sid 19350 */
/* reference: cve "2010-2876"; */
static RuleReference rule19350ref1 = 
{
    "cve", /* type */
    "2010-2876" /* value */
};

/* reference: url "www.adobe.com/support/security/bulletins/apsb10-20.html"; */
static RuleReference rule19350ref2 = 
{
    "url", /* type */
    "www.adobe.com/support/security/bulletins/apsb10-20.html" /* value */
};

static RuleReference *rule19350refs[] =
{
    &rule19350ref1,
    &rule19350ref2,
    NULL
};
/* metadata for sid 19350 */
/* metadata:; */


static RuleMetaData *rule19350metadata[] =
{
    NULL
};

RuleOption *rule19350options[] =
{
    &rule19350option0,
    &rule19350option1,
    &rule19350option2,
    &rule19350option3,
    NULL
};

Rule rule19350 = {
   /* rule header, akin to => tcp any any -> any any */
   {
       IPPROTO_TCP, /* proto */
       "$EXTERNAL_NET", /* SRCIP     */
       "$HTTP_PORTS", /* SRCPORT   */
       0, /* DIRECTION */
       "$HOME_NET", /* DSTIP     */
       "any", /* DSTPORT   */
   },
   /* metadata */
   { 
       3,  /* genid */
       19350, /* sigid */
       1, /* revision */
       "attempted-user", /* classification */
       0,  /* hardcoded priority XXX NOT PROVIDED BY GRAMMAR YET! */
       "WEB-CLIENT Adobe Shockwave Player Director file FFFFFF88 record integer overflow attempt",     /* message */
       rule19350refs /* ptr to references */
       ,rule19350metadata
   },
   rule19350options, /* ptr to rule options */
   &rule19350eval, /* use the built in detection function */
   0 /* am I initialized yet? */
};


/* detection functions */
int rule19350eval(void *p) {
   const u_int8_t *cursor_normal = 0;
   SFSnortPacket *sp = (SFSnortPacket *) p;

   const u_int8_t *beg_of_payload, *end_of_payload;
   const u_int8_t *cursor_detect;

   u_int16_t string_len;
   u_int32_t size, count, tmpval, tmpval2;

   DEBUG_SO(printf("rule19350eval enter\n");)

   if(sp == NULL)
      return RULE_NOMATCH;

   if(sp->payload == NULL)
      return RULE_NOMATCH;
   
   // flow:established, to_client;
   if(checkFlow(p, rule19350options[0]->option_u.flowFlags) <= 0)
      return RULE_NOMATCH;

   // flowbits:isset "http.dir";
   if(processFlowbits(p, rule19350options[1]->option_u.flowBit) <= 0)
      return RULE_NOMATCH;

   DEBUG_SO(printf("rule19350eval flow and flowbit check passed.\n");)

   if(getBuffer(sp, CONTENT_BUF_NORMALIZED, &beg_of_payload, &end_of_payload) <= 0)
      return RULE_NOMATCH;

   // Initialize our loop
   cursor_normal = beg_of_payload;

   // content:"|88 FF FF FF|", depth 0, fast_pattern;
   while(contentMatch(p, rule19350options[2]->option_u.content, &cursor_normal) > 0) {
      cursor_detect = cursor_normal;

      // content:"Shockwave 3D", distance:-20, within 12;
      // If this matches, it's not the record we're looking for so loop
      if(contentMatch(p, rule19350options[3]->option_u.content, &cursor_detect) > 0) {
         DEBUG_SO(printf("Found Shockwave 3D tag.  Skipping.\n");)
         continue;
      }

      // 4 + 2 + [0] + 30 + 4 + 4
      if(cursor_detect + 44 > end_of_payload)
         return RULE_NOMATCH;

      cursor_detect += 4; // skip chunk size
      string_len = READ_LITTLE_16(cursor_detect);
      cursor_detect += 2 + string_len + 30; // strlen, string, unk data

      if(cursor_detect + 8 > end_of_payload)
         return RULE_NOMATCH;

      size = READ_LITTLE_32(cursor_detect);
      cursor_detect += 4;
      count = READ_LITTLE_32(cursor_detect);

      DEBUG_SO(printf("Found record at offset 0x%08x.  strlen=0x%04x, size=0x%08x, count=0x%08x\n", (u_int32_t)(cursor_normal - beg_of_payload - 4), string_len, size, count);)

      // Vuln is if count * size * 0x18 overflows 32 bits.
      tmpval = size * count;
      if(tmpval / size != count)
         return RULE_MATCH;

      tmpval2 = tmpval * 0x18;
      if(tmpval2 / 0x18 != tmpval)
         return RULE_MATCH;

      // At this point, we've verified at a minimum that the payload looks
      // like this: "Shockwave 3d<8 bytes>|88 FF FF FF|".  This means that at
      // a minimum, we need to skip ahead 24 bytes before our "Shockwave"
      // content match has a chance to match.
      cursor_normal += 24;
   }

   return RULE_NOMATCH;
}
/*
Rule *rules[] = {
    &rule19350,
    NULL
};
*/
