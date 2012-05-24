#include "PlotTools.h"
#include "SusyScan.h"
#include "GeneratorMasses.h"

#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <cstring>
#include "TGraph.h"
#include "TF1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TObjArray.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TRint.h"
#include "TROOT.h"
#include "TLatex.h"

template<class T>
TGraph * PlotTools<T>::Line(double(*x)(const T*), double(*y)(const T*), double(*func)(const T*),
	const double mass, const double diff) {
	TGraph * result = new TGraph(1);
	std::sort(_scan->begin(), _scan->end(), sort_by(x));
	int i = 0;
	for (typename std::vector<T*>::const_iterator it = _scan->begin(); it != _scan->end(); ++it) {
		if (fabs(func(*it) - mass) < diff)
			result->SetPoint(i++, x(*it), y(*it));
	}
	result->SetLineWidth(1);
	result->SetLineColor(kGray);
	return result;
}

template<class T>
double PlotTools<T>::SingleValue(double(*func)(const T*)) {
	double result = 0;
	for (typename std::vector<T*>::const_iterator it = _scan->begin(); it != _scan->end(); ++it) {
		result = fabs(func(*it));
		if (result != 0)
			break;
	}

	return result;
}

template<class T>
double PlotTools<T>::roundDouble(double xin, int n) {
	double d = pow(10.0, n);
	double x = floor(xin * d + 0.5) / d;
	//	if(x==0){
	//		double d=pow(10.0, 3);
	//		x=floor(xin*d+0.5)/d;
	//	}
	return x;
}

template<class T>
double PlotTools<T>::SingleValue(double(*func)(const T*),double(*x)(const T*), double(*y)(const T*),double xValue, double yValue) {
	double result = 0;
	for (typename std::vector<T*>::const_iterator it = _scan->begin(); it != _scan->end(); ++it) {
		if(x(*it)==xValue && y(*it)==yValue){
			result = func(*it);

			//result = roundDouble(result);
			break;
		}
	}

	return result;
}

template<class T>
double PlotTools<T>::LimitValue(double(*x)(const T*), double(*y)(const T*), TGraph* limit, double xValue) {
	double yValue = limit->Eval(xValue);
	//std::cout<<"yValue!!!!!!!!!!!!!"<<yValue<<std::endl;
	//std::cout<<"xValue!!!!!!!!!!!!!"<<xValue<<std::endl;
	double resultYValue = 9999;
	for (typename std::vector<T*>::const_iterator it = _scan->begin(); it != _scan->end(); ++it) {
		//std::cout << "xValue=========" << x(*it) << std::endl;

		if (x(*it) == xValue) {
			//std::cout << "yValue=========" << y(*it) << std::endl;
			if (y(*it) >= yValue) {
				//std::cout << "!!!!!!!!!!!!yValue=========" << y(*it) << std::endl;
				if (y(*it) <= resultYValue) {
					resultYValue = y(*it);
					//std::cout << "new=========" << resultYValue << std::endl;
				}
			}
		}
	}
	return resultYValue;
}


void ComputeRange(TGraph * graph,Double_t &xmin, Double_t &ymin, Double_t &xmax, Double_t &ymax,bool rejectXGreaterY=false)
{
   // Compute the x/y range of the points in this graph
   if (graph->GetN() <= 0) {
      xmin = xmax = ymin = ymax = 0;
      return;
   }
   if(rejectXGreaterY)std::cout<<"!!!!!!!!!!!neutralino!!!!!!!!!!!!!!!!!!!!!!!!! :"<<rejectXGreaterY<<std::endl;
   Double_t*	fX=graph->GetX();
   Double_t*	fY=graph->GetY();
   xmin = xmax = -1;
   ymin = ymax = -1;
   for (Int_t i = 1; i < graph->GetN(); i++) {
  	 if(!rejectXGreaterY || fX[i]<=fY[i])
  		{
  		std::cout<<"!!!!!!!!!!!fX:"<<fX[i]<<std::endl;
  		std::cout<<"!!!!!!!!!!!fY:"<<fY[i]<<std::endl;

      if (xmin==-1 || fX[i] < xmin) xmin = fX[i];
      if (xmax==-1 || fX[i] > xmax) xmax = fX[i];
      if (ymin==-1 || fY[i] < ymin) ymin = fY[i];
      if (ymax==-1 || fY[i] > ymax) ymax = fY[i];
  		 }
   }
}

template<class T>
std::pair<double,double> PlotTools<T>::MinLimitValues( double(*x)(const T*),double(*y)(const T*),TGraph* limit,bool rejectXGreaterY) {
  Double_t xmin,xmax,ymin,ymax;
  ComputeRange(limit,xmin,ymin,xmax,ymax,rejectXGreaterY);
  double minx=xmin;
  double miny=ymin;
  double minLimitx=0;
  double minLimity=0;

  for (typename std::vector<T*>::const_iterator it = _scan->begin(); it != _scan->end(); ++it) {
  		if(x(*it)>=minLimitx && x(*it)<=minx){
  			minLimitx=x(*it);
  		}
  		if(y(*it)>=minLimity && y(*it)<=miny ){
  		  			minLimity=y(*it);
  		 }
  	}
    std::pair<double,double> res(minLimitx,minLimity);
	return res;
}

template<class T>
std::pair<double,double> PlotTools<T>::MaxLimitValues( double(*x)(const T*),double(*y)(const T*),TGraph* limit,bool rejectXGreaterY) {
  Double_t xmin,xmax,ymin,ymax;
  ComputeRange(limit,xmin,ymin,xmax,ymax,rejectXGreaterY);
  double maxx=xmax;
  double maxy=ymax;
  double maxLimitx=0;
  double maxLimity=0;
  //std::cout<<"!!!!!!!!!!!range x:"<<xmin<<" - "<<xmax<<std::endl;
  //std::cout<<"!!!!!!!!!!!range y:"<<ymin<<" - "<<ymax<<std::endl;
  //std::cout<<"!!!!!!!!!!!neutralino? :"<<rejectXGreaterY<<std::endl;
  for (typename std::vector<T*>::const_iterator it = _scan->begin(); it != _scan->end(); ++it) {
		if (x(*it) >= maxLimitx && x(*it) <= maxx) {
			maxLimitx = x(*it);
		}
		if (y(*it) >= maxLimity && y(*it) <= maxy) {
			maxLimity = y(*it);
		}
	}
 // std::cout<<"!!!!!!!!!!!range x:"<<maxLimitx<<std::endl;
  //std::cout<<"!!!!!!!!!!!range y:"<<maxLimity<<std::endl;
    std::pair<double,double> res(maxLimitx,maxLimity);
	return res;
}

void fillXSLimitAboveForInvalidResultPoints(TH2*h,TH2*hAcc){
	for (int x = 0; x <= h->GetXaxis()->GetNbins(); ++x) {

	for (int y = 0; y <= h->GetYaxis()->GetNbins(); ++y) {
		if (h->GetBinContent(x, y) >9000 ){
			for (int newy = y; newy <= h->GetYaxis()->GetNbins(); ++newy) {

			if (h->GetBinContent(x, newy) <9000 ){
//						std::cout<<"Old point!! x="<<x<<" y="<<y<<"Acceptance: "<<hAcc->GetBinContent(x, y)<<std::endl;
//						std::cout<<"New point!! x="<<x<<" y="<<newy<<"Acceptance: "<<hAcc->GetBinContent(x, newy)<<std::endl;
//						std::cout<<"New point!! XS old:"<<h->GetBinContent(x, newy)<<std::endl;
						double newxs=h->GetBinContent(x, newy)*hAcc->GetBinContent(x, y)/hAcc->GetBinContent(x, newy);
//						std::cout<<"Old point!! XS new (scaled):"<<newxs<<std::endl;
						//h->SetBinContent(x,y,newxs);
						break;
					}
			}

		}


		}
	}

};


bool isExcludedAbove(TH2*h, int x, int minyexcl) {

	// std::cout<<"is excluded? x="<<x<<" y="<<minyexcl<<std::endl;
	for (int y = minyexcl; y <= h->GetYaxis()->GetNbins()/2; ++y) {
//
		if (h->GetBinContent(x, y) != 1){
			//std::cout<<"YES!! x="<<x<<" y="<<y<<std::endl;
			return true;
		}

	}
	//std::cout<<"NO!! "<<std::endl;
	return false;
}
void FillEmptyPointsForNeutralinoScan(TH2*h) {
	for (int x = 0; x <= h->GetXaxis()->GetNbins(); ++x){
		for (int y = 0; y <= h->GetYaxis()->GetNbins(); ++y){
			if(isExcludedAbove(h,x,y) ){
				h->SetBinContent(x, y, 0.01);
			}
			else{
				h->SetBinContent(x, y, 1);
			}
		}
	}


}
template<class T>
void PlotTools<T>::SetMaxYPointAfterSmooth(TGraph* g,double minXNew,double maxYNew){

  	double xFormaxY=0;
  	double yForminX=0;
  	double yFormaxX=0;
  	double maxY=0;
  	double maxX=0;
  	double minX=9999;
  	for (int j = 0; j < (int)g->GetN(); ++j) {
  				double gx, gy;
  				g->GetPoint(j, gx, gy);
  				if(gy>maxY ){
  					maxY=gy;
  					xFormaxY=gx;
  				}
  				if(gx<minX ){
  					minX=gx;
  					yForminX=gy;
  				  				}
  				if(gx>maxX){
  					maxX=gx;
  				  					yFormaxX=gy;
  				  				  				}
  	}
  	//only set point for squark-gluino-scan!!!
  	if(minX>350){
  	 g->SetPoint(g->GetN(),xFormaxY,maxYNew);
  	 g->SetPoint(0,maxYNew,yFormaxX);
  	}
  	else{
  		 g->SetPoint(g->GetN(),minXNew,yForminX);
  	}
  }

void FillEmptyPointsForLowMassPoints(TH2*h) {
	for (int x = 0; x <= h->GetXaxis()->GetNbins(); ++x){
		for (int y = 0; y <= h->GetYaxis()->GetNbins()/2; ++y){
			if(isExcludedAbove(h,x,y) ){
				h->SetBinContent(x, y, 0.01);
			}
			else{
				h->SetBinContent(x, y, 1);
			}
		}
	}


}
void FillOverflowRowToImprovePlotting(TH2*h) {
	for (int x = 0; x <= h->GetXaxis()->GetNbins(); ++x){
		for (int y =h->GetNbinsY()+1; y >= h->GetNbinsY()-1; --y){

			if(h->GetBinContent(x, y-1) != 1 ){
				h->SetBinContent(x, y, 0.01);
			}
			else{
				h->SetBinContent(x, y, 1);
			}
		}
	}


}

void RejectHighExcludedPointsPerHand(TH2*h) {
	for (int x = 0; x <=  h->GetXaxis()->GetNbins(); ++x){
		for (int y = 50; y <=  h->GetYaxis()->GetNbins(); ++y){

				h->SetBinContent(x, y, 1);

		}
	}


}
void RejectFailedPointsPerHand(TH2*h) {
	for (int x = 0; x <=  h->GetXaxis()->GetNbins(); ++x){
		for (int y = 0; y <=  h->GetYaxis()->GetNbins(); ++y){
     if(h->GetBinContent(x, y)==0){
    	// std::cout  << ": x=" << x << ", y=" << y <<"h->GetBinContent(x, y) "<<h->GetBinContent(x, y)<< std::endl;
				h->SetBinContent(x, y, 1);
     }
		}
	}


}

template<class T>
void PlotTools<T>::Area(TH2*h, double(*x)(const T*), double(*y)(const T*), double(*f)(const T*)) {
	//std::cout << _scan->size() << ",  &scan=" << _scan << std::endl;
	for (typename std::vector<T*>::const_iterator it = _scan->begin(); it != _scan->end(); ++it) {
		if(f(*it)<9000){
		h->SetBinContent(h->GetXaxis()->FindBin(x(*it)), h->GetYaxis()->FindBin(y(*it)), f(*it));
		}
		//std::cout  << ": x=" << x(*it) << ", y=" << y(*it) << std::endl;
	}

}

template<class T>
void PlotTools<T>::Graph(TGraph*g, double(*x)(const T*), double(*y)(const T*), double ymin) {
	unsigned i = g->GetN();
	std::sort(_scan->begin(), _scan->end(), sort_by(x));
	for (typename std::vector<T*>::const_iterator it = _scan->begin(); it != _scan->end(); ++it) {
		if (y(*it) >= ymin)
			g->SetPoint(i++, x(*it), y(*it));
		std::cout << i << ": x=" << x(*it) << ", y=" << y(*it) << std::endl;
	}
}

template<class T>
std::vector<TGraph*> PlotTools<T>::GetContours005(TH2*h, int ncont) {
	double conts[1];
	conts[0] = 0.05;
	if (!h)
		return std::vector<TGraph*>();
	TH2 * plot = (TH2*) h->Clone();
	plot->SetContour(1, conts);
	plot->SetFillColor(1);
	plot->Draw("CONT Z List");
	gPad->Update();
	TObjArray *contours = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
	int ncontours = contours->GetSize();
	std::vector<TGraph*> result;
	for (int i = 0; i < ncontours; ++i) {
		TList *list = (TList*) contours->At(i);
		TGraph* curv = (TGraph*) list->First();
		if (curv)
			result.push_back(curv);
		for (int j = 0; j < list->GetSize(); j++) {
			curv = (TGraph*) list->After(curv); // Get Next graph
			if (curv)
				result.push_back(curv);
		}
	}
	delete plot;
	std::sort(result.begin(), result.end(), sort_TGraph());
	return result;
}

template<class T>
TGraph * PlotTools<T>::GetContour005(TH2*h, int ncont, int flag) {
	return (TGraph*) GetContours005(h, ncont).at(flag)->Clone();
}

template<class T>
std::vector<TGraph*> PlotTools<T>::GetContours(TH2*h, int ncont,bool excludeBelowExcludedRegion,bool isAsymptoticLimit) {
	if (!h)
		return std::vector<TGraph*>();
	TH2 * plot = (TH2*) h->Clone();
	RejectFailedPointsPerHand(plot);
	if(excludeBelowExcludedRegion){
	 FillEmptyPointsForNeutralinoScan(plot);
	}
	if(isAsymptoticLimit){
	 // RejectHighExcludedPointsPerHand(plot);
	  //FillEmptyPointsForNeutralinoScan(plot);
	  FillEmptyPointsForLowMassPoints(plot);
	}
	//else{FillEmptyPointsForLowMassPoints(plot);}
	//RejectHighExcludedPointsPerHand(plot);
	plot->SetContour(ncont);
	plot->SetFillColor(1);
	plot->Draw("CONT Z List");
	gPad->Update();
	TObjArray *contours = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
	if (!contours) {
		std::cerr << "ERROR: Found no contours! Is the histogram empty?" << std::endl;
		return std::vector<TGraph*>();
	}
	int ncontours = contours->GetSize();
	std::vector<TGraph*> result;
	for (int i = 0; i < ncontours; ++i) {
		TList *list = (TList*) contours->At(i);
		TGraph* curv = (TGraph*) list->First();
		if (curv)
			result.push_back(curv);
		for (int j = 0; j < list->GetSize(); j++) {
			curv = (TGraph*) list->After(curv); // Get Next graph
			if (curv)
				result.push_back(curv);
		}
	}
	delete plot;
	std::sort(result.begin(), result.end(), sort_TGraph());
	return result;
}

template<class T>
TGraph * PlotTools<T>::GetContour(TH2*h, int ncont, int flag,bool excludeBelowExcludedRegion,bool isAsymptoticLimit) {
	std::vector<TGraph*> gs = GetContours(h, ncont,excludeBelowExcludedRegion,isAsymptoticLimit);
	//assert(gs.size()>flag && "ERROR: Requested a non-existing contour index! Check with ExclusionTestContours first!");
	return (gs.size() > flag ? (TGraph*) gs[flag]->Clone() : new TGraph(0));
}


template<class T>
TGraph * PlotTools<T>::GetContour(TH2*h, double(*x)(const T*), double(*y)(const T*), double(*func)(
	const T*), int ncont, int flag, int color, int style,bool excludeBelowExcludedRegion,bool isAsymptoticLimit) {
	TH2*hist = (TH2*) h->Clone();
	Area(hist, x, y, func);

	TGraph * graph = GetContour(hist, ncont, flag, excludeBelowExcludedRegion,isAsymptoticLimit);
	graph->SetLineColor(color);
	graph->SetLineStyle(style);
	return graph;
}

template<class T>
TGraph * PlotTools<T>::GetContour005(TH2*h, double(*x)(const T*), double(*y)(const T*),
	double(*func)(const T*), int ncont, int flag, int color, int style) {
	TH2*hist = (TH2*) h->Clone();
	Area(hist, x, y, func);
	TGraph * graph = GetContour005(hist, ncont, flag);
	graph->SetLineColor(color);
	graph->SetLineStyle(style);
	return graph;
}

template<class T>
void PlotTools<T>::Print(double(*f)(const T*), double(*x)(const T*), double(*y)(const T*),
	TGraph*g, double p) {
	for (typename std::vector<T*>::const_iterator it = _scan->begin(); it != _scan->end(); ++it) {
		for (int j = 0; j < g->GetN(); ++j) {
			double gx, gy;
			g->GetPoint(j, gx, gy);
			if ((x(*it) - gx) * (x(*it) - gx) + (y(*it) - gy) * (y(*it) - gy) < p * p)
				std::cout << x(*it) << ", " << y(*it) << " :: " << f(*it) << std::endl;
		}
	}

}
template<class T>
void PlotTools<T>::Print(double(*f)(const T*), double(*x1)(const T*), double(*x2)(const T*),
	double(*x3)(const T*), double(*x4)(const T*), TGraph*g, double p) {
	for (typename std::vector<T*>::const_iterator it = _scan->begin(); it != _scan->end(); ++it) {
		for (int j = 0; j < g->GetN(); ++j) {
			double gx, gy;
			g->GetPoint(j, gx, gy);
			if ((x1(*it) - gx) * (x1(*it) - gx) + (x2(*it) - gy) * (x2(*it) - gy) < p * p)
				std::cout << x1(*it) << ", " << x2(*it) << ", " << x3(*it) << ", " << x4(*it)
					<< " :: " << f(*it) << std::endl;
		}
	}

}

template<class T>
TGraph * PlotTools<T>::ChooseBest(TGraph*r1, TGraph*r2, TGraph*g1, TGraph*g2, double x, double y) {
	TGraph * res = new TGraph(0);
	for (int i = 0; i < r1->GetN(); ++i) {
		double rx, r1y;
		r1->GetPoint(i, rx, r1y);
		double r2y = r2->Eval(rx);
		double g1y = g1->Eval(rx);
		double resy = (r1y >= r2y ? g1y : g2->Eval(rx));
		if (rx > x && r1y > y)
			res->SetPoint(i, rx, resy);
		else
			res->SetPoint(i, rx, g1y);
	}
	res->SetLineColor(g1->GetLineColor());
	res->SetLineWidth(g1->GetLineWidth());
	res->SetLineStyle(g1->GetLineStyle());
	res->SetFillColor(g1->GetFillColor());
	res->SetFillStyle(g1->GetFillStyle());
	return res;
}

template<class T>
TH2 * PlotTools<T>::BinWiseOr(TH2*h1, TH2*h2) {
	TH2 * res = (TH2*) h1->Clone();
	for (int x = 0; x <= res->GetXaxis()->GetNbins(); ++x)
		for (int y = 0; y <= res->GetYaxis()->GetNbins(); ++y)
			if (h2->GetBinContent(x, y) > 0.5)
				res->SetBinContent(x, y, h2->GetBinContent(x, y));
	return res;
}

template<class T>
TGraph * PlotTools<T>::ModifyExpSigma(TGraph*g1, TGraph*g2, TGraph*g3) {
	TGraph * res = new TGraph(0);
	for (int i = 0; i < g1->GetN(); ++i) {
		double x, y;
		g1->GetPoint(i, x, y);
		double y2 = g2->Eval(x);
		double y3 = g3->Eval(x);
		res->SetPoint(i, x, y - y2 + y3);
	}
	res->SetLineColor(g1->GetLineColor());
	res->SetLineStyle(g1->GetLineStyle());
	res->SetLineWidth(g1->GetLineWidth());
	res->SetFillColor(g1->GetFillColor());
	res->SetFillStyle(g1->GetFillStyle());
	return res;
}

double GetX(TGraph* g, double x0, double y0) {
	double dn = 99999999, up = 999999999, minx, maxx;
	for (int i = 0; i < g->GetN(); ++i) {
		double x, y;
		g->GetPoint(i, x, y);
		if (y == y0)
			return x;
		if (y < y0 && (x - x0) * (x - x0) + (y - y0) * (y - y0) < dn) {
			dn = (x - x0) * (x - x0) + (y - y0) * (y - y0);
			minx = x;
		}
		if (y > y0 && (x - x0) * (x - x0) + (y - y0) * (y - y0) < up) {
			up = (x - x0) * (x - x0) + (y - y0) * (y - y0);
			maxx = x;
		}
	}
	return (minx + maxx) / 2.;
}

template<class T>
TGraph * PlotTools<T>::ModifyExpSigmaY(TGraph*g1, TGraph*g2, TGraph*g3) {
	TGraph * res = new TGraph(0);
	for (int i = 0; i < g1->GetN(); ++i) {
		double x, y;
		g1->GetPoint(i, x, y);
		double x2, y2, x3, y3;
		x2 = GetX(g2, x, y);
		x3 = GetX(g3, x, y);
		res->SetPoint(i, x - x2 + x3, y);
	}
	res->SetLineColor(g1->GetLineColor());
	res->SetLineStyle(g1->GetLineStyle());
	res->SetLineWidth(g1->GetLineWidth());
	res->SetFillColor(g1->GetFillColor());
	res->SetFillStyle(g1->GetFillStyle());
	return res;
}

template<class T>
bool PlotTools<T>::sort_TGraph::operator()(const TGraph*g1, const TGraph*g2) {
	return g1->GetN() > g2->GetN();
}
void DrawNeutrNNLSP(double max) {
	TGraph*gluinoNLSP = new TGraph(0);
	gluinoNLSP->SetPoint(0, 50, 50);
	gluinoNLSP->SetPoint(1, max, max);
	gluinoNLSP->SetPoint(2, max, 50);
	gluinoNLSP->SetPoint(3, 50, 50);
	gluinoNLSP->SetFillColor(kGray);
	gluinoNLSP->Draw("f");

	TLatex tex;
	tex.SetTextSize(0.05);
	tex.SetTextFont(62);
	tex.SetNDC(true);
	tex.DrawLatex(0.6, 0.25, "#tilde{g} NLSP");
	gPad->RedrawAxis();
}
TGraph * GetExcludedRegion(TGraph * lowerLimit, double min1, double min2, double max1, double max2) {
	TGraph* excludedRegion = new TGraph(lowerLimit->GetN() + 3);
	int nbins = lowerLimit->GetN();
double maxXgrpoint=0;
double minXgrpoint=9999;
double maxYgrpoint=0;
	for (int i = 0; i < nbins + 1; i++) {
		double x, y;
		if(maxXgrpoint<x)maxXgrpoint=x;
		if(minXgrpoint<x)minXgrpoint=x;
		if(maxYgrpoint<y)maxYgrpoint=y;
		lowerLimit->GetPoint(i, x, y);
		excludedRegion->SetPoint(i, x, y);
		if (i == nbins) {
			excludedRegion->SetPoint(nbins, min1, y);
		}
	}

	//excludedRegion->SetPoint(nbins,2000.1,400);
	// excludedRegion->SetPoint(nbins+2,min1,max2);
	excludedRegion->SetPoint(nbins + 2, min1, min2);
	excludedRegion->SetPoint(nbins + 3, maxXgrpoint, min2);
	//excludedRegion->SetPoint(nbins+3,400,2000.1);


	excludedRegion->SetFillStyle(1001);
	excludedRegion->SetFillColor(kBlue-10);
	return excludedRegion;

}
TGraph * MakeBand(TGraph *g1, TGraph *g2, bool b) {
	//std::cout<<"MAKE BAND!"<<b<<std::endl;
	TGraph * res = new TGraph(g1->GetN() + g2->GetN() + 1);
	int p = 0;
	double firstx, firsty;
	for (int i = 0; i < g1->GetN(); ++i) {
		double x, y;
		g1->GetPoint(i, x, y);
		res->SetPoint(p++, x, y);
		if (i == 0) {
			firstx = x;
			firsty = y;
		}
	}
	for (int i = g2->GetN() - 1; i >= 0; --i) {
		double x, y;
		g2->GetPoint(i, x, y);
		res->SetPoint(p++, x, y);
	}
	res->SetPoint(p++, firstx, firsty);
	if (res->GetN() == 0)
		return res;
	res->SetLineColor(g2->GetLineColor());
	res->SetFillColor(g2->GetLineColor());
	res->SetFillStyle(4050);
	return res;
}
std::string getStringFromInt(int i) {
	char result[100];
	sprintf(result, "%d", i);
	std::string ret = result;
	return " " + ret;
	//---------
}
void drawCmsPrelInCanvas(double intLumi) {
	TLatex* lat = new TLatex(0.46, 0.84, "CMS");
	lat->SetNDC(true);
	lat->SetTextSize(0.04);
	lat->Draw("same");

	TLatex* lat2 = new TLatex(0.46, 0.77, Form("#sqrt{s} = 7 TeV,  #int#it{L}dt = %.2ffb^{-1}",
												intLumi/1000.));
	lat2->SetNDC(true);
	lat2->SetTextSize(0.04);
	lat2->Draw("same");
}
void drawCmsPrel(double intLumi, std::string METCut, int noJets, bool isBestjet, std::string jetLabel, bool drawChannelInfo, bool drawCMS) {
	//std::cout<<"draw cms prel"<<std::endl;
	intLumi=intLumi/1000;

		TLatex as;
		as.SetNDC(true);
		as.SetTextColor(12);
		as.SetTextFont(43);
		as.SetTextSize(20);
		//as.SetTextFont(42);//ms.SetTextColor(12);
		std::string out = "";
//		if (drawChannelInfo) {
//			out = "#int #font[12]{L}dt = %.1ffb^{  -1}  #sqrt{s} = 7 TeV";
//		}
//		else{
//			out = "                                     #sqrt{s} = 7 TeV";
//		}
//
//			as.DrawLatex(0.16, 0.94, Form(out.c_str(), intLumi));
		if (drawChannelInfo) {
						//out = "#int #font[12]{L}dt = %.1ffb^{  -1}  #sqrt{s} = 7 TeV";
						out = "%.1ffb^{  -1}  #sqrt{s} = 7 TeV";
					}
					else{
						out = "                                     #sqrt{s} = 7 TeV";
						out = "               #sqrt{s} = 7 TeV";
					}

						//as.DrawLatex(0.28, 0.905, Form(out.c_str(), intLumi));
						as.DrawLatex(0.45, 0.905, Form(out.c_str(), intLumi));

		as.SetTextSize(20);
		as.SetTextColor(1);
		if (drawChannelInfo) {
			//1#gamma, >=3 jets, MET>"+getStringFromInt(METCut)+" GeV ";
			out = "#geq1#gamma, #geq" + getStringFromInt(noJets) + " jets";
			if (isBestjet)
				out = "#geq1#gamma, #geq2/3 jets";
			if (jetLabel != "")
				out = jetLabel;
//			if (METCut != "") {
//
//				if (isdigit(METCut[0])) {
//					out = out + ", #slash{E}_{T} #geq " + METCut + " GeV";
//				} else {
//					out = out + ", " + METCut + "";
//				}
//			}
			as.SetTextSize(20);

		//	as.DrawLatex(0.56, 0.905, Form(out.c_str()));
			as.DrawLatex(0.75, 0.905, Form(out.c_str()));

		}



		if(drawCMS){
		 as.SetTextSize(20);
		 //as.DrawLatex(0.0, 0.905, "CMS preliminary");
		 as.DrawLatex(0.20, 0.905, "CMS");
		 as.SetTextSize(20);
		 //as.DrawLatex(0.0, 0.93, "preliminary");
		}
}

void Smooth(TGraph * g, int N, int flag) {
	TGraph * old = (TGraph*) g->Clone();
	//int N = (n%2==0?n+1:n);
	if (N > 2 * g->GetN())
		N = 2 * g->GetN() - 1;

	double gauss[N];
	double sigma = (double) N / 4.;
	double sum = 0;
	double lim = (double) N / 2.;
	TF1 *fb = new TF1("fb", "gaus(0)", -lim, lim);
	fb->SetParameter(0, 1. / (sqrt(2 * 3.1415) * sigma));
	fb->SetParameter(1, 0);
	fb->SetParameter(2, sigma);
	for (int i = 0; i < N; ++i) {
		gauss[i] = fb->Integral(-lim + i, -lim + i + 1);
		sum += gauss[i];
	}
	for (int i = 0; i < N; ++i)
		gauss[i] /= sum;

	for (int i = 0; i < g->GetN(); ++i) {
		double avy = 0., avx = 0., x, x0, y, y0;
		int points = 0;
		for (int j = i - N / 2; j <= i + N / 2; ++j) {
			if (j < 0) {
				old->GetPoint(0, x, y);
		        }		
			else if (j >= g->GetN()) {
				old->GetPoint(old->GetN() - 1, x, y);
			}	
			else 
			  old->GetPoint(j, x, y);
			avy += y * gauss[points];
			avx += x * gauss[points];
			
			if (i == j) {
				x0 = x;
				y0 = y;
			}	
			++points;
		}
		if      ((flag==1 && i - N / 2 < 0 ) || (flag==2 && i + N / 2 >= g->GetN()))
			g->SetPoint(i, x0, avy);
		else if ((flag==1 && i + N / 2 >= g->GetN()) || (flag==2 && i - N / 2 < 0 ))
			g->SetPoint(i, avx, y0);
		else
			g->SetPoint(i, avx, avy);
	}
	delete old;	
}

void Smooth2D(TGraph * g, int N) {
	TGraph * old = Close2D(g);
	if (N > 2 * g->GetN())
		N = 2 * g->GetN() - 1;

	double gauss[N];
	double sigma = (double) N / 4.;
	double sum = 0;
	double lim = (double) N / 2.;
	TF1 *fb = new TF1("fb", "gaus(0)", -lim, lim);
	fb->SetParameter(0, 1. / (sqrt(2 * 3.1415) * sigma));
	fb->SetParameter(1, 0);
	fb->SetParameter(2, sigma);
	for (int i = 0; i < N; ++i) {
		gauss[i] = fb->Integral(-lim + i, -lim + i + 1);
		sum += gauss[i];
	}
	for (int i = 0; i < N; ++i)
		gauss[i] /= sum;

	for (int i = 0; i < g->GetN(); ++i) {
		double avy = 0., avx = 0, x, x0, y;
		int points = 0;
		for (int j = i - N / 2; j <= i + N / 2; ++j) {
			//if      (j<0)          old->GetPoint(old->GetN()+j, x, y);
			//else if (j>=g->GetN()) old->GetPoint(j-old->GetN(), x, y);
			if (j < 0)
				old->GetPoint(0, x, y);
			else if (j >= g->GetN())
				old->GetPoint(old->GetN() - 1, x, y);
			else
				old->GetPoint(j, x, y);
			if (i == j)
				x0 = x;
			avy += y * gauss[points];
			avx += x * gauss[points];
			++points;
		}
		g->SetPoint(i, avx, avy);
	}
	delete old;
}

TGraph * Close2D(TGraph * g) {
	TGraph * f = new TGraph(0);
	if (g->GetN() == 0)
		return f;
	double x, y;
	g->GetPoint(0, x, y);
	g->SetPoint(g->GetN(), x, y);

	int i = 0;
	for (; i < g->GetN(); ++i) {
		g->GetPoint(i, x, y);
		//if (x<450&&y<450) break;
	}
	int p = 0;
	for (int j = i + 1; j != i; ++j) {
		if (j >= g->GetN())
			j = 0;
		g->GetPoint(j, x, y);
		//if (y<110+(x-120)*390/442||(x<330&&y<1000)||(x<500&&y<600)) continue;
		f->SetPoint(p++, x, y);
	}
	return f;
}

template class PlotTools<SusyScan> ;
template class PlotTools<GeneratorMasses> ;
