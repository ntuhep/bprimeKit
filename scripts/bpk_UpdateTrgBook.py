#!/bin/env python2
#*************************************************************************
#
#  Filename    : bpk_UpdateTrgBook.py
#  Description : Updating an existing HLT List
#  Author      : Pu-Sheng Chen [ sam7k9621@hep1.phys.ntu.edu.tw ]
#
#*************************************************************************

import argparse
import sys
import os

bpkutil_path = os.environ['CMSSW_BASE'] + '/src/bpkFrameWork/bprimeKit/interface/'

TrgBook_h ="""\
#ifndef __TRIGGERBOOKING_H__
#define __TRIGGERBOOKING_H__

#define N_TRIGGER_BOOKINGS {0}

//update to {1}

const std::string TriggerBooking[N_TRIGGER_BOOKINGS] = {{

    {2}

}};

enum TriggerBitNumber {{

    {3}

}};

#endif // __TRIGGERBOOKING_H__
"""


def main(args):
    parser = argparse.ArgumentParser(
            "Options for updating TriggerBooking.h",
            formatter_class=argparse.ArgumentDefaultsHelpFormatter
            )

    # Operation to perform
    parser.add_argument(
            '-f', '--inputfile',
            help='Input trigger list file',
            type=str, default=None, required=True
            )

    parser.add_argument(
            '-v', '--version',
            help='Input the latest version you have added in trigger list file',
            type=str, default=None, required=True
            )

    parser.add_argument(
            '-o', '--outputfile',
            help='Output file in interface',
            type=str, default= bpkutil_path + "TriggerBooking.h"
            )

    try:
        opt = parser.parse_args(args[1:])
    except:
        print "Error processing arguments!"
        parser.print_help()
        raise

    ## Transform the lines from HLT list into output.h
    with open(opt.inputfile) as listfile:
        content = listfile.readlines()

    content = [c.replace('\n', '') for c in content]

    content_dec = [" \"{}\"".format(c) for c in content]
    declare_line = ",\n    ".join(content_dec)


    content_mod = []
    for idx, trg in enumerate(content):
        line = "{0} = {1}".format( trg, idx )
        content_mod.append(line)

    enum_line =  ",\n    ".join(content_mod)

    with open(opt.outputfile, "w") as hfile :
        hfile.write( TrgBook_h.format( str(len(content)), opt.version, declare_line, enum_line ) )

    print "Success updating/making file in interface"


if __name__ == '__main__':
    main(sys.argv)
