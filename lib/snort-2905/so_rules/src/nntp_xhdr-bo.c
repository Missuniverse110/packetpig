/*
 * NNTP XHDR buffer overflow attempt
 * 
 * Copyright (C) 2007 Sourcefire, Inc. All Rights Reserved
 * 
 * Writen by Patrick Mullen <pmullen@sourcefire.com>
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

/* declare detection functions */
int ruleNNTP_XHDR_BOeval(void *p);

static RuleReference ruleNNTP_XHDR_BOref0 = 
{
    "url", /* type */
    "www.microsoft.com/technet/security/bulletin/ms07-056.mspx" /* value */
};
static RuleReference ruleNNTP_XHDR_BOref1 =
{
    "cve", /* type */
    "2007-3897"
};

static RuleReference *ruleNNTP_XHDR_BOrefs[] =
{
    &ruleNNTP_XHDR_BOref0,
    &ruleNNTP_XHDR_BOref1,
    NULL
};

static FlowFlags ruleNNTP_XHDR_BOflow =
{
    FLOW_ESTABLISHED|FLOW_TO_CLIENT
};

static RuleOption ruleNNTP_XHDR_BOoption0 =
{
    OPTION_TYPE_FLOWFLAGS,
    {
        &ruleNNTP_XHDR_BOflow
    }
};

static ContentInfo ruleNNTP_XHDR_BOcontent0 =
{
    (u_int8_t *)"211",                 /* pattern to search for */
    3,                      /* depth */
    0,                      /* offset */
    CONTENT_BUF_NORMALIZED | CONTENT_NOCASE,                      /* flags */
    NULL,                   /* holder for boyer/moore info */
    NULL,                   /* holder for byte representation of "NetBus" */
    0,                      /* holder for length of byte representation */
    0                       /* holder of increment length */
};

static RuleOption ruleNNTP_XHDR_BOoption1 =
{
    OPTION_TYPE_CONTENT,
    {
        &ruleNNTP_XHDR_BOcontent0
    }
};

RuleOption *ruleNNTP_XHDR_BOoptions[] =
{
    &ruleNNTP_XHDR_BOoption0,
    &ruleNNTP_XHDR_BOoption1,
    NULL
};

Rule ruleNNTP_XHDR_BO = {
   /* rule header */
   {
       IPPROTO_TCP, /* proto */
       EXTERNAL_NET, /* SRCIP     */
       "119", /* SRCPORT   */
       0, /* DIRECTION */
       HOME_NET, /* DSTIP     */
       "any", /* DSTPORT   */
   },
   /* metadata */
   { 
       3,  /* genid (HARDCODED!!!) */
       12636, /* sigid 441ad88b-8be7-4969-88a5-cf17bdb63172 */
       3, /* revision c72a2f58-fcc3-4c2b-b365-958f2a3df598 */
   
       "attempted-user", /* classification, generic */
       0,  /* hardcoded priority XXX NOT PROVIDED BY GRAMMAR YET! */
       "NNTP XHDR buffer overflow attempt",     /* message */
       ruleNNTP_XHDR_BOrefs /* ptr to references */
        ,NULL
   },
   ruleNNTP_XHDR_BOoptions, /* ptr to rule options */
   &ruleNNTP_XHDR_BOeval, /* ptr to rule detection function */
   0, /* am I initialized yet? */
   0, /* number of options */
   0  /* don't alert */
};


/* detection functions */
/* this detects a malformed 211 response wherein the number of
 * messages in the given range doesn't match the number of
 * messages given ...
 * the detection of extra XHDR response messages beyond the
 * number of messages in the 211 is still TBD
 */

int ruleNNTP_XHDR_BOeval(void *p) {

   const u_int8_t *cursor_normal, *beg_of_payload, *end_of_payload;
   const u_int8_t *strtoul_start, *strtoul_end;

//   const u_int8_t truf[] = "yeah, we know ><";
	   
   u_int32_t num_messages, first_msg, last_msg;  

   SFSnortPacket *sp = (SFSnortPacket *) p;

   if(sp == NULL)
      return RULE_NOMATCH;

   if(sp->payload == NULL)
      return RULE_NOMATCH;


   /* call flow match */
   if(checkFlow(sp, ruleNNTP_XHDR_BOoptions[0]->option_u.flowFlags) <= 0 )
      return RULE_NOMATCH;

   if(contentMatch(sp, ruleNNTP_XHDR_BOoptions[1]->option_u.content, &cursor_normal) <= 0)
            return RULE_NOMATCH;

   if(getBuffer(sp, CONTENT_BUF_NORMALIZED, &beg_of_payload, &end_of_payload) <= 0)
      return RULE_NOMATCH;

   if((end_of_payload - beg_of_payload) < 11)   /* Minimum length */
      return RULE_NOMATCH;

   /* Get the number of messages */
   strtoul_start = cursor_normal;
   strtoul_end = end_of_payload;
   num_messages = strtoul((const char *)strtoul_start, (char**)&strtoul_end, 10);

   /* RULE_NOMATCH if we ran out of buffer or didn't read any digits */
   if(!strtoul_end || strtoul_end >= end_of_payload || strtoul_end == strtoul_start)
      return RULE_NOMATCH;

   /* Get the first message number */
   strtoul_start = strtoul_end;
   strtoul_end = end_of_payload;
   first_msg = strtoul((const char *)strtoul_start, (char**)&strtoul_end, 10);

   if(!strtoul_end || strtoul_end >= end_of_payload || strtoul_end == strtoul_start)
       return RULE_NOMATCH;
   
   /* Get the last message number */
   strtoul_start = strtoul_end;
   strtoul_end = end_of_payload;
   last_msg = strtoul((const char *)strtoul_start, (char**)&strtoul_end, 10);

   /* There should still be the forum name after this number */
   if(!strtoul_end || strtoul_end >= end_of_payload || strtoul_end == strtoul_start)
      return RULE_NOMATCH;

   /* Something's screwy, but it's not our problem */
   if(last_msg < first_msg)
      return RULE_NOMATCH;

   /* Verify number of messages */
   if(last_msg - first_msg + 1 > num_messages)
      return RULE_MATCH;

   return RULE_NOMATCH;
}

