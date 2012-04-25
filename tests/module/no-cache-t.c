/*
 * Authentication tests for the pam-krb5 module without a ticket cache.
 *
 * This test case includes tests that require Kerberos to be configured and a
 * username and password available, but which don't write a ticket cache
 * (which requires additional work to test the cache ownership).  This test
 * does not set AUTHTOK.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2011, 2012
 *     The Board of Trustees of the Leland Stanford Junior University
 *
 * See LICENSE for licensing terms.
 */

#include <config.h>
#include <portable/system.h>

#include <tests/fakepam/script.h>
#include <tests/tap/kerberos.h>
#include <tests/tap/process.h>
#include <tests/tap/string.h>


int
main(void)
{
    struct script_config config;
    struct kerberos_config *krbconf;

    /* Load the Kerberos principal and password from a file. */
    krbconf = kerberos_setup(TAP_KRB_NEEDS_PASSWORD);
    memset(&config, 0, sizeof(config));
    config.user = krbconf->userprinc;
    config.password = krbconf->password;
    config.extra[0] = krbconf->username;
    config.extra[1] = "bogus.example.com";

    /*
     * Generate a testing krb5.conf file with a nonexistent default realm so
     * that we can be sure that our principals will stay fully-qualified in
     * the logs.
     */
    kerberos_generate_conf("bogus.example.com");

    plan_lazy();
    run_script_dir("data/scripts/no-cache", &config);

    return 0;
}
