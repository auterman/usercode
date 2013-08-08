#! /usr/bin/env python

import ROOT

from messageLogger import messageLogger as log
from optparse import OptionParser

def parseSLHA(slha, pdgIds=[24]):
	"""
	parse SUSY Les Houches Accord (SLHA) formated strings (e.g. SOFTSUSY output)
	"""
	from re import match
	result = {}
	massBlock = ""
	block = None
	for line in slha.splitlines():		
		blockRe = match("Block ([a-zA-Z]+).*", line)
		if not blockRe == None:
			block = blockRe.group(1)
		if block == "MASS":
			for pdgId in pdgIds:
				pdgIdRe = match("\s*%s\s*([0-9.e\+-]+)"%pdgId, line)
				if not pdgIdRe == None:
					result[pdgId] = float(pdgIdRe.group(1))
	for pdgId in pdgIds:
		if not pdgId in result:
			log.logError("could not find %s"%pdgId)
			log.logDebug(slha)
	return result

def runSoftsusy(parameters):
	"""
	parameters: dict holding all information needed to run SOSTUSY for one point
	
	result: dict of resulting mass spectrum
	"""
	from subprocess import Popen, PIPE
	#softpoint.x                 sugra     <m0>  <m12>  <a0>  <tanb>  <mgut>  <sgnMu>
	template = "%(path)s %(mode)s %(m0)s %(m12)s %(A0)s %(tanb)s %(mgut)s %(sgnMu)s"
	log.logDebug(template%parameters)
	proc = Popen(template%parameters, shell=True, stdout=PIPE, stderr=PIPE)
	slha = proc.communicate()[0]
	return slha

def run():
	from ROOT import TH2F, TCanvas, gStyle
	from itertools import product
	susyParameters = {
		"path":"softsusy-3.3.4/softpoint.x",
		"mode": "sugra",
		"m0": 60,
		"m12": 250,
		"A0":-2969,
		"tanb":39.,
		"mgut":"unified",
		"sgnMu":1
					  }
	nBinsM0 = 5
	nBinsM12 = 2
#	m0Range = (1000,5000)
#	m12Range = (1000,5000)
	m0Range = (4500,6000)
	m12Range = (200,700)

	print "Hi!"
	
	higgsMassOK = TH2F("higgs",";m_{0};m_{1/2}", nBinsM0, m0Range[0], m0Range[1], nBinsM12,  m12Range[0], m12Range[1])
	edgeMassOK = TH2F("edge",";m_{0};m_{1/2}", nBinsM0, m0Range[0], m0Range[1], nBinsM12,  m12Range[0], m12Range[1])
	for xBin, yBin in product(range(1,nBinsM0+1), range(1,nBinsM12+1)):
		pdgIds = [25, 1000022, 1000023]
		susyParameters["m0"]   = higgsMassOK.GetXaxis().GetBinCenter(xBin)
		susyParameters["m12"] = edgeMassOK.GetYaxis().GetBinCenter(yBin)
		
		print susyParameters
		susyPoint = runSoftsusy(susyParameters)
		print susyPoint
#		spectrum = parseSLHA(susyPoint, pdgIds)
#		print susyParameters
#               print spectrum
		

		deltaNeutralinoMass = 0.
		if 1000023 in spectrum and 1000022 in spectrum:
			deltaNeutralinoMass = spectrum[1000023] - spectrum[1000022]
			
		if 60. < deltaNeutralinoMass and deltaNeutralinoMass < 90.:
			edgeMassOK.Fill(susyParameters["m0"], susyParameters["m12"], deltaNeutralinoMass)

		if 25 in spectrum and 120. < spectrum[25] and spectrum[25] < 127.:
			higgsMassOK.Fill(susyParameters["m0"], susyParameters["m12"], abs(spectrum[25]-125.))
			print susyParameters["m0"], susyParameters["m12"], deltaNeutralinoMass, spectrum[25]

	gStyle.SetOptStat(0)
	gStyle.SetPalette(1)
	canv = 	TCanvas("canv","canv",800,600)
	higgsMassOK.Draw("COLZ")
	canv.Print("higgsMass.png")
	waitForInput()
	edgeMassOK.Draw("COLZ")
	canv.Print("edgeMass.png")
	waitForInput()
	
	

# This method just waits for a button to be pressed
def waitForInput():
    raw_input("Press any key to continue!")
    return

# entry point
#-------------
if (__name__ == "__main__"):
    print "Start!"
    # use option parser to allow verbose mode
    parser = OptionParser()
    parser.add_option("-v", "--verbose", dest="verbose", action="store_true", default=False,
                                  help="talk about everything")
    (opts, args) = parser.parse_args()
    if (opts.verbose):
        # print out all output
        log.outputLevel = 5
    else:
        # ignore output with "debug" level
        log.outputLevel = 4

    print "Start!"
    # start
    run()
 
