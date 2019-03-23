/*
 * httpping.c
 *
 * Copyright (c) Ben Sampson <htttp://github.com/billyrayvalentine>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <popt.h>
#include <curl/curl.h>

void bail(CURLcode res);

int main(int argc, char *argv[])
{
  // command line args
  int arg_num_of_pings = 0;
  int arg_machine_readable = 0;
  int arg_follow_redirects = 0;
  const char *arg_url;

  int keep_pinging = 1;
  int ping_forever = 0;
  int ping_count = 0;
  long response_code, header_size;
  double total_time;
  // curl_off_t total;
  char *effective_url;
  CURL *c_handle;
  CURLcode res;

  poptContext pc;
  struct poptOption poptoptions[] = {
    {"number-pings", 'c', POPT_ARG_INT, &arg_num_of_pings, 0, "number of times to ping", "COUNT"},
    {"follow-redirects", 'f', POPT_ARG_NONE, &arg_follow_redirects, 0, "Follow HTTP 3XX redirects", NULL},
    {"machine-readable", 'm', POPT_ARG_NONE, &arg_machine_readable, 0, "machine friendly output", NULL},
		POPT_AUTOHELP
    POPT_TABLEEND
  };

  pc = poptGetContext(NULL, argc, (const char **)argv, poptoptions, 0);

  poptSetOtherOptionHelp(pc, "<url> [-cfmh?]");

	int opt;
	opt = poptGetNextOpt(pc);


  // Process any errors
  if (opt < -1) {
    fprintf(stderr, "%s: %s\n",
      poptBadOption(pc, POPT_BADOPTION_NOALIAS),
      poptStrerror(opt));
      poptPrintHelp(pc, stderr, 0);
      exit(EXIT_FAILURE);
  }

  // Get the URL arg - only except one
  if (! poptPeekArg(pc)) {
    fprintf(stderr, "missing url\n");
      poptPrintHelp(pc, stderr, 0);
      exit(EXIT_FAILURE);
  }
  else {
    arg_url = poptGetArg(pc);
    // Any remaining args are ignored
  }

  /*
  printf("Got options num_of_pings = %d, url = %s, follow_redirects=%d, machine_readable=%d",
      arg_num_of_pings, arg_url, arg_follow_redirects, arg_machine_readable);
  printf(" opt =%d\n", opt);
  */

  poptFreeContext(pc);

  // Main loop
  while (keep_pinging) {
    c_handle = curl_easy_init();
    curl_easy_setopt(c_handle, CURLOPT_URL, arg_url);

    if (arg_follow_redirects) {
      curl_easy_setopt(c_handle, CURLOPT_FOLLOWLOCATION, 1L);
    }

    // Just do a HEAD request
    curl_easy_setopt(c_handle, CURLOPT_NOBODY, 1L);

    // Do the request
    res = curl_easy_perform(c_handle);

    if (res != CURLE_OK) {
      bail(res);
    }

    res = curl_easy_getinfo(c_handle, CURLINFO_RESPONSE_CODE, &response_code);

    if (res != CURLE_OK) {
      bail(res);
    }

    res = curl_easy_getinfo(c_handle, CURLINFO_EFFECTIVE_URL, &effective_url);

    if (res != CURLE_OK) {
      bail(res);
    }

    res = curl_easy_getinfo(c_handle, CURLINFO_HEADER_SIZE, &header_size);

    if (res != CURLE_OK) {
      bail(res);
    }

    res = curl_easy_getinfo(c_handle, CURLINFO_TOTAL_TIME, &total_time);
    //res = curl_easy_getinfo(c_handle, CURLINFO_TOTAL_TIME_T, &total);

    if (res != CURLE_OK) {
      bail(res);
    }

    ping_count++;

    if (arg_machine_readable) fprintf(stdout,
                                  "%ld %s %ld %.1f %d\n",
                                  response_code,
                                  effective_url,
                                  header_size,
                                  total_time,
                                  ping_count);

    else
      fprintf(stdout, "%ld from %s header size = %ld in %.1f seconds seq=%d\n",
                 response_code, effective_url, header_size, total_time, ping_count);
    /*else
      fprintf(stdout, "%ld from %s header size = %ld in %ld seconds\n",
                          response_code, effective_url, header_size, (long)total);
    */

    curl_easy_cleanup(c_handle);
    sleep(1);

    if (ping_forever == 0 && ping_count == arg_num_of_pings)
      keep_pinging = 0;
  }
  return EXIT_SUCCESS;
}

void bail(CURLcode curl_code)
{
  fprintf(stderr, "failed: %s\n", curl_easy_strerror(curl_code));
  exit(EXIT_FAILURE);
}
