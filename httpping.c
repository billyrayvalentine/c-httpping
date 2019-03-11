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
#include <getopt.h>
#include <curl/curl.h>

void usage(void);
void bail(CURLcode res);

int main(int argc, char *argv[])
{
  // command line args
  int num_of_pings = 1;
  int machine_readable = 0;
  int ping_forever = 1;
  int follow_redirects = 0;

  int opt;
  int keep_pinging = 1;
  int ping_count = 0;
  long response_code, header_size;
  double total_time;
  // curl_off_t total;
  char *url;
  char *effective_url;
  CURL *c_handle;
  CURLcode res;


  // Get args
  if (argc < 2) {
    usage();
    exit(1);
  }

  url = argv[1];

  while ((opt = getopt(argc, argv, "n:mfh")) != -1) {
    switch (opt) {

    case 'n':
      num_of_pings = atoi(optarg);
      ping_forever = 0;
      break;

    case 'm':
      machine_readable = 1;
      break;

    case 'f':
      follow_redirects = 1;
      break;

    case 'h':
      usage();
      exit(0);

    case '?':
      usage();
      exit(1);
    }
  }

  if (!(num_of_pings >0)) {
    fprintf(stderr, "Invalid number_of_pings\n");
    usage();
    exit(1);
  }


  /*
     if (! c_handle) {
     printf("Could not setup curl, exiting");
     exit(1);
     }
   */

  // Main loop
  while (keep_pinging) {
    c_handle = curl_easy_init();
    curl_easy_setopt(c_handle, CURLOPT_URL, url);

    if (follow_redirects) {
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

    if (machine_readable) fprintf(stdout,
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

    if (ping_forever == 0 && ping_count == num_of_pings)
      keep_pinging = 0;
  }
  return 0;
}

void usage()
{
  printf("Usage: httpping <url> [-nfmh]\n");
  printf("\t-n number_of_pings - number of times to ping.  Default = Ping forever\n");
  printf("\t-f follow_redirects - follow HTTP 3xx redirects.  Default = 0\n");
  printf("\t-m machine_readable - output in a machine friendly format.  Default = 0\n");
  printf("\t-h - this message\n");
}

void bail(CURLcode curl_code)
{
  fprintf(stderr, "failed: %s\n", curl_easy_strerror(curl_code));
  exit(1);
}
