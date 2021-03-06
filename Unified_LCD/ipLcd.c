/*
Copyright (C) Dougie Lawson 2015-2017, all rights reserved.
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ipLcd.h"

char *
getIPaddr ()
{

  struct ifaddrs *interfaceArray = NULL, *tempIfAddr = NULL;
  void *tempAddrPtr = NULL;
  int rc = 0;
  char addressOutputBuffer[INET6_ADDRSTRLEN];
  char *ifaceIP = malloc (40);

  rc = getifaddrs (&interfaceArray);	/* retrieve the current interfaces */
  if (rc == 0)
    {
      for (tempIfAddr = interfaceArray; tempIfAddr != NULL;
	   tempIfAddr = tempIfAddr->ifa_next)
	{
	  if (tempIfAddr->ifa_addr->sa_family == AF_INET)
	    {
	      tempAddrPtr =
		&((struct sockaddr_in *) tempIfAddr->ifa_addr)->sin_addr;

	      if (strncmp (tempIfAddr->ifa_name, "lo", 2))	/* interface ISN'T loopback */
		{
		  inet_ntop (tempIfAddr->ifa_addr->sa_family, tempAddrPtr,
			     addressOutputBuffer,
			     sizeof (addressOutputBuffer));
		  sprintf (ifaceIP, "%s %s\0", tempIfAddr->ifa_name,
			   addressOutputBuffer);
		}
	    }
	}

      freeifaddrs (interfaceArray);	/* free the dynamic memory */
      interfaceArray = NULL;	/* prevent use after free  */
    }
  else
    {
      printf ("getifaddrs() failed with errno =  %d %s \0",
	      errno, strerror (errno));
    }

  return ifaceIP;
}
