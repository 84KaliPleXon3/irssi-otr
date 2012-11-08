#include "otr.h"
char *otr_help = "%9- OTR help -%9\nQuick start:  Do \"%_/load otr%_\"  (irssi syntax), write  \"%_?OTR?%_\" to your  OTR buddy,\nwait until the now ongoing key  generation finishes and write \"%_?OTR?%_\" again. You\nshould \"%_go secure%_\".\n\nKey generation happens in a separate  process and its duration mainly depends on\nthe available entropy. On  my desktop it takes about 6  Minutes, about 2 Minutes\nif I run  \"%_du /%_\" in parallel  and on an idle  server system it can  even take an\nhour.\n\nThe  default  OTR  policy  of  irc-otr  is  now  something  between  manual  and\nopportunistic. Manual  means you have to  start it yourself by  issuing \"%_?OTR?%_\",\nopportunistic means  both peers send  some magic  whitespace and start  OTR once\nthey receive this whitespace from the other side. irc-otr uses a mode in between\nwhere we are not sending whitespace as an announcement (as in opportunistic) but\nwe still  handle whitespace if  we see  it from the  other side (I'm  calling it\nhandlews). Therefore if your peer  uses opportunistic the handshake should still\nstart automatically once he writes something.\n\nYou can  now set the  OTR policy  per peer via  the otr_policy /setting.  It's a\ncomma separated  list of \"%_<nick>@<server> <policy>%_\"  pairs where <nick>@<server>\nis interpreted as a glob pattern, i.e. you can use wildcard \"%_*%_\" and joker \"%_?%_\" as\nyou would  in a shell.  The policy  can be one  of never, manual,  handlews (the\ndefault),  opportunistic, and  always. Be  aware that  the opportunistic  policy\nfails with  some IRC  servers since  they strip off  the whitespace.  The always\npolicy has  the nice side effect  that the first  line you type will  already be\nencrypted.\n\nIf  a  fingerprint can  be  found  for someone,  i.e.  someone  you had  an  OTR\nconversation  with  before,  then  the otr_policy_known  setting  applies  after\notr_policy. It has the same syntax. The  default is \"%_* always%_\", i.e. enforce OTR\nwith anyone you've used OTR with before.\n\nShould you finish an OTR session via  \"%_/otr finish%_\" and should the active policy\nbe always  or opportunistic then  it will be  temporarily set back  to handlews.\nOtherwise OTR would start again right away  which is probably not what you want.\nThis is however reset once you close the query window.\n\nTo make  sure that you are  actually talking to your  buddy, you can agree  on a\nsecret somehow  and then one does  \"%_/otr auth <secret>%_\". Shortly  afterwards the\nother  one will  be  asked to  do  the  same and  you're  done. The  traditional\nalternative, comparing  fingerprints over a secure  line, can also be  used. Use\n\"%_/otr trust%_\" once you're sure they match.\n\nI also  strongly recommend to do  \"%_/statusbar window add otr%_\"  (irssi syntax) so\nyou're informed about what's going on.\n\nIn \"%_<client config dir>/otr/otr.{key,fp}%_\" you'll  find the fingerprints and your\nprivate keys(should you at any point be interested).\n\nCommands:\n\n%_/otr genkey nick@irc.server.com %_\n        Manually generate a key for the given account(also done on demand)\n%_/otr auth [<nick>@<server>] <secret>%_\n        Initiate or respond to an authentication challenge\n%_/otr authabort [<nick>@<server>] %_\n        Abort any ongoing authentication\n%_/otr trust [<nick>@<server>]%_\n        Trust the fingerprint of the user in the current window.\n        You should only do this after comparing fingerprints over a secure line\n%_/otr debug%_\n        Switch debug mode on/off\n%_/otr contexts%_\n        List all OTR contexts along with their fingerprints and status\n%_/otr finish [<nick>@<server>]%_\n        Finish an OTR conversation\n%_/otr version%_\n        Display irc-otr version. Might be a git commit\n\nSettings:\n\n%_otr_policy%_\n        Comma-separated list of \"%_<nick>@<server> <policy>%_\" pairs. See comments\n        above.\n%_otr_policy_known%_\n        Same syntax as otr_policy. Only applied where a fingerprint is\n        available.\n%_otr_ignore%_\n        Conversations with nicks that match this regular expression completely\n        bypass libotr. It is very unlikely that you need to touch this setting,\n        just use the OTR policy never to prevent OTR sessions with some nicks.\n%_otr_finishonunload%_\n        If true running OTR sessions are finished on /unload and /quit.\n%_otr_createqueries%_\n        If true queries are automatically created for OTR log messages.\n%9- End of OTR help -%9";
FORMAT_REC formats[] = {
	{ MODULE_NAME, "otr", 0}
	,
		{ NULL, "Keys", 0 }
	,
		{ "kg_failed", "%9OTR%9: Key generation for $0: failed: $1 ($2)", 3, { 0, 0, 0 }},
		{ "kg_completed", "%9OTR%9: Key generation for $0: completed in $1 seconds. Reloading keys", 2, { 0, 1 }},
		{ "kg_aborted_dup", "%9OTR%9: Key generation for $0: aborted. Key generation for $1 still in progress", 2, { 0, 0 }},
		{ "kg_aborted_dir", "%9OTR%9: Key generation for $0: aborted, failed creating directory $1: $2", 3, { 0, 0, 0 }},
		{ "kg_mkdir", "%9OTR%9: created directory $0", 1, { 0 }},
		{ "kg_pipe", "%9OTR%9: Key generation for $0: error creating pipe: $1", 2, { 0, 0 }},
		{ "kg_fork", "%9OTR%9: Key generation for $0: fork() error: $1", 2, { 0, 0 }},
		{ "kg_initiated", "%9OTR%9: Key generation for $0: initiated. This might take several minutes or on some systems even an hour. If you wanna check that something is happening, see if there are two processes of your IRC client.", 1, { 0 }},
		{ "kg_exited", "%9OTR%9: Key generation for $0: child terminated for unknown reason", 1, { 0 }},
		{ "kg_exitsig", "%9OTR%9: Key generation for $0: child was killed by signal $1", 2, { 0, 0 }},
		{ "kg_pollerr", "%9OTR%9: Key generation for $0: error poll()ing child: $1", 2, { 0, 0 }},
		{ "kg_abort", "%9OTR%9: Key generation for $0: aborted", 1, { 0 }},
		{ "kg_needacc", "%9OTR%9: I need an account name. Try something like /otr genkey mynick@irc.server.net", 0},
		{ "kg_noabort", "%9OTR%9: No ongoing key generation to abort", 0},
		{ "key_not_found", "%9OTR%9: no private keys found", 0},
		{ "key_loaded", "%9OTR%9: private keys loaded", 0},
		{ "key_load_error", "%9OTR%9: Error loading private keys: $0 ($1)", 2, { 0, 0 }},
		{ NULL, "Fingerprints", 0 }
	,
		{ "fp_saved", "%9OTR%9: fingerprints saved", 0},
		{ "fp_save_error", "%9OTR%9: Error saving fingerprints: $0 ($1)", 2, { 0, 0 }},
		{ "fp_not_found", "%9OTR%9: no fingerprints found", 0},
		{ "fp_loaded", "%9OTR%9: fingerprints loaded", 0},
		{ "fp_load_error", "%9OTR%9: Error loading fingerprints: $0 ($1)", 2, { 0, 0 }},
		{ "fp_trust", "%9OTR%9: Trusting fingerprint from $0", 1, { 0 }},
		{ NULL, "Instance Tags", 0 }
	,
		{ "instag_saved", "%9OTR%9: instance tags saved", 0},
		{ "instag_save_error", "%9OTR%9: Error saving instance tags: $0 ($1)", 2, { 0, 0 }},
		{ "instag_not_found", "%9OTR%9: no instance tags found", 0},
		{ "instag_loaded", "%9OTR%9: instance tags loaded", 0},
		{ "instag_load_error", "%9OTR%9: Error loading instance tags: $0 ($1)", 2, { 0, 0 }},
		{ NULL, "Callbacks", 0 }
	,
		{ "ops_notify_bug", "%9OTR%9: BUG() in ops_notify", 0},
		{ "ops_notify", "%9OTR%9: title: $0 prim: $1 sec: $2", 3, { 0, 0, 0 }},
		{ "ops_display_bug", "%9OTR%9: BUG() in ops_display", 0},
		{ "ops_display", "%9OTR%9: msg: $0", 1, { 0 }},
		{ "ops_sec", "%9OTR%9: gone %9secure%9", 0},
		{ "ops_fpcomp", "%9OTR%9: Your peer is not authenticated. To make sure you're talking to the right guy you can either agree on a secret and use the authentication described in %9/otr auth%9, or use the traditional way and compare fingerprints over a secure line (e.g. telephone) and subsequently enter %9/otr trust%9.  Your fingerprint is: $0. $1's fingerprint: $2", 3, { 0, 0, 0 }},
		{ "ops_insec", "%9OTR%9: gone %9insecure%9", 0},
		{ "ops_still_reply", "%9OTR%9: still %9secure%9 (is reply)", 0},
		{ "ops_still_no_reply", "%9OTR%9: still %9secure%9 (is not reply)", 0},
		{ "ops_log", "%9OTR%9: log msg: $0", 1, { 0 }},
		{ "ops_inject", "%9OTR%9: Couldn't inject message from $0 for $1: $2", 3, { 0, 0, 0 }},
		{ "ops_handle_msg", "%9OTR%9: Message event $0, msg $1", 2, { 0, 0 }},
		{ NULL, "SendingReceiving", 0 }
	,
		{ "send_failed", "%9OTR%9: send failed: msg=$0", 1, { 0 }},
		{ "send_change", "%9OTR%9: couldn't find context also OTR changed the outgoing message(BUG?)", 0},
		{ "send_fragment", "%9OTR%9: failed to fragment message: msg=$0", 1, { 0 }},
		{ "send_converted", "%9OTR%9: OTR converted sent message to $0", 1, { 0 }},
		{ "receive_ignore_query", "%9OTR%9: ignoring rest of OTR default query msg", 0},
		{ "receive_dequeued", "%9OTR%9: dequeued msg of length $0", 1, { 1 }},
		{ "receive_queued", "%9OTR%9: queued msg of length $0", 1, { 1 }},
		{ "receive_ignore", "%9OTR%9: ignoring protocol message of length $0, acc=$1, from=$2: $3", 4, { 1, 0, 0, 0 }},
		{ "receive_converted", "%9OTR%9: OTR converted received message", 0},
		{ "otr_better_two", "%9OTR%9: <b>$0</b> has requested an <a href=\"http://otr.cypherpunks.ca/\">Off-the-Record private conversation</a>.  However, you do not have a plugin to support that.", 1, { 0 }},
		{ "otr_better_three", "%9OTR%9: See <a href=\"http://otr.cypherpunks.ca/\">http://otr.cypherpunks.ca/</a> for more information.", 0},
		{ NULL, "Context", 0 }
	,
		{ "ctx_not_found", "%9OTR%9: couldn't find context: acc=$0 nick=$1", 2, { 0, 0 }},
		{ "ctx_not_create", "%9OTR%9: couldn't create/find context: acc=$0 from=$1", 2, { 0, 0 }},
		{ NULL, "Authentication", 0 }
	,
		{ "auth_aborted_ongoing", "%9OTR%9: Ongoing authentication aborted", 0},
		{ "auth_aborted", "%9OTR%9: Authentication aborted", 0},
		{ "auth_responding", "%9OTR%9: Responding to authentication request...", 0},
		{ "auth_initiated", "%9OTR%9: Initiated authentication...", 0},
		{ "auth_have_old", "%9OTR%9: $0 wanted to authenticate but an old authentication was still ongoing.  Old authentication will be aborted, please try again.", 1, { 0 }},
		{ "auth_peer", "%9OTR%9: $0 wants to authenticate. Type /otr auth <your-shared-secret> to complete.", 1, { 0 }},
		{ "auth_peer_qa", "%9OTR%9: $0 wants to authenticate and asked you \"$1\". Type /otr auth <answer> to complete.", 2, { 0, 0 }},
		{ "auth_peer_reply_wrong", "%9OTR%9: $0 replied to an auth we didn't start.", 1, { 0 }},
		{ "auth_peer_replied", "%9OTR%9: $0 replied to our auth request...", 1, { 0 }},
		{ "auth_peer_wrong_smp3", "%9OTR%9: $0 sent a wrong authentication message (SMP3).", 1, { 0 }},
		{ "auth_peer_wrong_smp4", "%9OTR%9: $0 sent a wrong authentication message (SMP4).", 1, { 0 }},
		{ "auth_successful", "%9OTR%9: %GAuthentication successful!%n", 0},
		{ "auth_failed", "%9OTR%9: %RAuthentication failed!%n", 0},
		{ "auth_needenc", "%9OTR%9: You need to establish an OTR session before you can authenticate.", 0},
		{ NULL, "Commands", 0 }
	,
		{ "cmd_otr", "%9OTR%9: We're alive", 0},
		{ "cmd_qnotfound", "%9OTR%9: Failed: Can't get nick and server of current query window. (Or maybe you're doing this in the status window?)", 0},
		{ "cmd_auth", "%9OTR%9: Please agree on a secret with your peer and then initiate the authentication with /otr auth <secret> or let him initiate. Should you initiate your peer will after a little while be instructed to enter the secret as well. Once he has done so the authentication will finish up. Should you have both typed in the same secret the authentication should be successful.", 0},
		{ "cmd_debug_on", "%9OTR%9: Debug mode is on", 0},
		{ "cmd_debug_off", "%9OTR%9: Debug mode is off", 0},
		{ "cmd_finish", "%9OTR%9: Finished conversation with $0@$1.", 2, { 0, 0 }},
		{ "cmd_finishall_none", "%9OTR%9: No conversations to finish.", 0},
		{ "cmd_version", "%9OTR%9: This is irc-otr version $0", 1, { 0 }},
		{ "peer_finished", "%9OTR%9: $0 has finished the OTR conversation. If you want to continue talking enter %9/otr finish%9 for plaintext or ?OTR? to restart OTR.", 1, { 0 }},
		{ NULL, "Contexts", 0 }
	,
		{ "ctx_ctx_unencrypted", "%9$[20]0%9    $[30]1    plaintext", 2, { 0, 0 }},
		{ "ctx_ctx_encrypted", "%9$[20]0%9    $[30]1    %gencrypted%n", 2, { 0, 0 }},
		{ "ctx_ctx_finished", "%9$[20]0%9    $[30]1    finished", 2, { 0, 0 }},
		{ "ctx_ctx_unknown", "%9$[20]0%9    $[30]1    unknown state(BUG?)", 2, { 0, 0 }},
		{ "ctx_fps_no", "$0 %rnot authenticated%n", 1, { 0 }},
		{ "ctx_fps_smp", "$0 %gauthenticated%n via shared secret (SMP)", 1, { 0 }},
		{ "ctx_fps_man", "$0 %gauthenticated%n manually", 1, { 0 }},
		{ "ctx_noctxs", "No active OTR contexts found", 0},
		{ NULL, "Statusbar", 0 }
	,
		{ "st_plaintext", "{sb plaintext}", 0},
		{ "st_untrusted", "{sb %GOTR%n (%runverified%n)}", 0},
		{ "st_trust_smp", "{sb %GOTR%n}", 0},
		{ "st_trust_manual", "{sb %GOTR%n}", 0},
		{ "st_smp_incoming", "{sb {%GOTR%n %9Auth requested%9}}", 0},
		{ "st_smp_outgoing", "{sb {%GOTR%n %9Waiting auth reply%9}}", 0},
		{ "st_smp_finalize", "{sb {%GOTR%n %9Finalizing auth%9}}", 0},
		{ "st_smp_unknown", "{sb {hilight unknown auth state!}}", 0},
		{ "st_finished", "{sb %yfinished%n}", 0},
		{ "st_unknown", "{sb {hilight state unknown (BUG!)}}", 0},
		{ NULL, NULL, 0 }
};
