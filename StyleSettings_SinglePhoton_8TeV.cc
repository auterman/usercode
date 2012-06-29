// $Id: StyleSettings_SinglePhoton_8TeV.cc,v 1.1 2012/06/26 13:58:23 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     TheBetterPlotScript, a tool to plot final results, limits and exclusion contours, based on 'PlotScript'

     Christian Autermann, Hamburg University/LPC, February 2012
 *** ------------------------------------------------------------------------------------------------------- ***/

#include <iostream>

#include "TString.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TGraph.h"

#include "StyleSettings.h"


TGraph* Obs_7TeV_Bino2j_squark_gluino();
TGraph* Exp_7TeV_Bino2j_squark_gluino();
TGraph* Obs_7TeV_Wino2j_squark_gluino();
TGraph* Exp_7TeV_Wino2j_squark_gluino();

void Draw_SqGlBino()
{
  TGraph * oldLimit = Exp_7TeV_Bino2j_squark_gluino();
  oldLimit->Draw("l");
}

style* SqGlBino_Style(){ /// Sq-Gl Bino /// ---------------------------------------------------------------------
  style * s = new style();
  s->leg = new TLegend(0.26, 0.26, 0.61, 0.45, "#splitline{GGM bino-like #tilde{#chi}^{0}}{m_{#tilde{#chi}^{0}} = 375 GeV}");
  s->leg->SetBorderSize(0);
  s->leg->SetLineColor(0);
  s->leg->SetFillColor(10);
  s->leg->SetFillStyle(1001);
  s->leg->SetTextFont(42);
  s->leg->SetTextSize(0.03);

  s->lumi = new TLatex(0.58, 0.901, ((std::string)"4.0fb^{  -1}  #sqrt{s} = 8 TeV   #geq1#gamma, #geq2 jets").c_str());
  s->lumi->SetNDC(true);
  s->lumi->SetTextColor(12);
  s->lumi->SetTextFont(43);
  s->lumi->SetTextSize(20);
  s->cms = new TLatex(0.21, 0.901, "#bf{CMS}");
  s->cms->SetNDC(true);
  s->cms->SetTextColor(12);
  s->cms->SetTextFont(43);
  s->cms->SetTextSize(20);
  s->cmsprelim = new TLatex(0.21, 0.901, "#bf{CMS preliminary}");
  s->cmsprelim->SetNDC(true);
  s->cmsprelim->SetTextColor(12);
  s->cmsprelim->SetTextFont(43);
  s->cmsprelim->SetTextSize(20);
  s->lumiTemperaturePlot = new TLatex(0.48, 0.906, ((std::string)"4.0fb^{  -1}  #sqrt{s} = 8 TeV   #geq1#gamma, #geq2 jets").c_str());
  s->lumiTemperaturePlot->SetNDC(true);
  s->lumiTemperaturePlot->SetTextColor(12);
  s->lumiTemperaturePlot->SetTextFont(43);
  s->lumiTemperaturePlot->SetTextSize(20);
  s->cmsTemperaturePlot = new TLatex(0.21, 0.906, "#bf{CMS}");
  s->cmsTemperaturePlot->SetNDC(true);
  s->cmsTemperaturePlot->SetTextColor(12);
  s->cmsTemperaturePlot->SetTextFont(43);
  s->cmsTemperaturePlot->SetTextSize(20);
  s->cmsprelimTemperaturePlot = new TLatex(0.21, 0.906, "#bf{CMS preliminary}");
  s->cmsprelimTemperaturePlot->SetNDC(true);
  s->cmsprelimTemperaturePlot->SetTextColor(12);
  s->cmsprelimTemperaturePlot->SetTextFont(43);
  s->cmsprelimTemperaturePlot->SetTextSize(20);

  s->excluded = new TLatex(0.3, 0.7, "excluded");
  s->excluded->SetNDC(true);
  s->excluded->SetTextColor(12);
  s->excluded->SetTextFont(43);
  s->excluded->SetTextSize(20);
  s->smooth_flag=0;
  s->smooth_points=25;
  s->coverUp=Draw_SqGlBino;
  s->iCLsObsExcl=0;  
  s->iCLsExpExcl=0;  
  s->iCLsExpExclm1=0;
  s->iCLsExpExclp1=0;
  s->iCLsObsTheom1=0;
  s->iCLsObsTheop1=0;
  s->iCLsExpTheom1=0;
  s->iCLsExpTheop1=0;  
  
  s->MinXsecZ=0.005;
  s->MaxXsecZ=0.05;  
  s->SetMoreLogLabels=true;

  return s;
}

void Draw_SqGlWino()
{
  TGraph * oldLimit = Exp_7TeV_Wino2j_squark_gluino();
  oldLimit->Draw("l");
}


style* SqGlWino_Style(){ /// Sq-Gl Wino /// ---------------------------------------------------------------------
  style * s = new style();
  s->leg = new TLegend(0.47, 0.65, 0.85, 0.83, "#splitline{GGM wino-like #tilde{#chi}^{0}}{m_{#tilde{#chi}^{0}} = 375 GeV}");
  s->leg->SetBorderSize(0);
  s->leg->SetLineColor(0);
  s->leg->SetFillColor(10);
  s->leg->SetFillStyle(1001);
  s->leg->SetTextFont(42);
  s->leg->SetTextSize(0.03);

  if (s->lumi) delete s->lumi;
  if (s->cms) delete s->cms;
  if (s->cmsprelim) delete s->cmsprelim;
  if (s->lumiTemperaturePlot) delete s->lumiTemperaturePlot;
  if (s->cmsTemperaturePlot) delete s->cmsTemperaturePlot;
  if (s->cmsprelimTemperaturePlot) delete s->cmsprelimTemperaturePlot;
  s->lumi = new TLatex(0.58, 0.901, ((std::string)"4.0fb^{  -1}  #sqrt{s} = 8 TeV   #geq1#gamma, #geq2 jets").c_str());
  s->lumi->SetNDC(true);
  s->lumi->SetTextColor(12);
  s->lumi->SetTextFont(43);
  s->lumi->SetTextSize(20);
  s->cms = new TLatex(0.21, 0.901, "#bf{CMS}");
  s->cms->SetNDC(true);
  s->cms->SetTextColor(12);
  s->cms->SetTextFont(43);
  s->cms->SetTextSize(20);
  s->cmsprelim = new TLatex(0.2, 0.901, "#bf{CMS preliminary}");
  s->cmsprelim->SetNDC(true);
  s->cmsprelim->SetTextColor(12);
  s->cmsprelim->SetTextFont(43);
  s->cmsprelim->SetTextSize(20);
  s->lumiTemperaturePlot = new TLatex(0.48, 0.906, ((std::string)"4.0fb^{  -1}  #sqrt{s} = 8 TeV   #geq1#gamma, #geq2 jets").c_str());
  s->lumiTemperaturePlot->SetNDC(true);
  s->lumiTemperaturePlot->SetTextColor(12);
  s->lumiTemperaturePlot->SetTextFont(43);
  s->lumiTemperaturePlot->SetTextSize(20);
  s->cmsTemperaturePlot = new TLatex(0.21, 0.906, "#bf{CMS}");
  s->cmsTemperaturePlot->SetNDC(true);
  s->cmsTemperaturePlot->SetTextColor(12);
  s->cmsTemperaturePlot->SetTextFont(43);
  s->cmsTemperaturePlot->SetTextSize(20);
  s->cmsprelimTemperaturePlot = new TLatex(0.21, 0.906, "#bf{CMS preliminary}");
  s->cmsprelimTemperaturePlot->SetNDC(true);
  s->cmsprelimTemperaturePlot->SetTextColor(12);
  s->cmsprelimTemperaturePlot->SetTextFont(43);
  s->cmsprelimTemperaturePlot->SetTextSize(20);
  s->excluded = new TLatex(0.3, 0.3, "excluded");
  s->excluded->SetNDC(true);
  s->excluded->SetTextColor(12);
  s->excluded->SetTextFont(43);
  s->excluded->SetTextSize(25);
  s->smooth_flag=0;
  s->smooth_points=25;
  s->coverUp=Draw_SqGlWino;
  s->iCLsObsExcl=0;  
  s->iCLsExpExcl=0;  
  s->iCLsExpExclm1=0;
  s->iCLsExpExclp1=0;
  s->iCLsObsTheom1=0;
  s->iCLsObsTheop1=0;
  s->iCLsExpTheom1=0;
  s->iCLsExpTheop1=0;  


  s->MinXsecZ=0.05;
  s->MaxXsecZ=0.5;  
  s->SetMoreLogLabels=true;

  
  return s;
}




TGraph* Obs_7TeV_Bino2j_squark_gluino()
{
   TGraph *graph = new TGraph(67);
   graph->SetName("Observed limit");
   graph->SetTitle("Observed limit");
   graph->SetFillColor(84);

   graph->SetLineColor(kBlack);
   graph->SetLineWidth(2);
   graph->SetPoint(0,1844.902,1029.652);
   graph->SetPoint(1,1832.294,1030.521);
   graph->SetPoint(2,1818.393,1031.69);
   graph->SetPoint(3,1803.339,1033.035);
   graph->SetPoint(4,1787.22,1034.453);
   graph->SetPoint(5,1770.131,1036.065);
   graph->SetPoint(6,1752.239,1037.847);
   graph->SetPoint(7,1733.708,1039.763);
   graph->SetPoint(8,1714.768,1041.943);
   graph->SetPoint(9,1695.663,1044.3);
   graph->SetPoint(10,1676.456,1046.905);
   graph->SetPoint(11,1657.343,1049.683);
   graph->SetPoint(12,1638.336,1052.729);
   graph->SetPoint(13,1619.585,1055.995);
   graph->SetPoint(14,1601.254,1059.606);
   graph->SetPoint(15,1583.405,1063.546);
   graph->SetPoint(16,1565.944,1067.963);
   graph->SetPoint(17,1548.94,1072.855);
   graph->SetPoint(18,1532.312,1078.371);
   graph->SetPoint(19,1516.136,1084.496);
   graph->SetPoint(20,1500.332,1091.352);
   graph->SetPoint(21,1485.064,1099.066);
   graph->SetPoint(22,1470.306,1107.557);
   graph->SetPoint(23,1455.963,1116.786);
   graph->SetPoint(24,1442.12,1126.881);
   graph->SetPoint(25,1428.919,1137.89);
   graph->SetPoint(26,1416.314,1149.664);
   graph->SetPoint(27,1404.186,1162.111);
   graph->SetPoint(28,1392.47,1175.382);
   graph->SetPoint(29,1381.389,1189.547);
   graph->SetPoint(30,1370.923,1204.228);
   graph->SetPoint(31,1360.934,1219.441);
   graph->SetPoint(32,1351.519,1235.218);
   graph->SetPoint(33,1342.587,1251.403);
   graph->SetPoint(34,1334.064,1268.075);
   graph->SetPoint(35,1326.029,1285.156);
   graph->SetPoint(36,1318.403,1302.725);
   graph->SetPoint(37,1311.266,1320.704);
   graph->SetPoint(38,1304.454,1338.998);
   graph->SetPoint(39,1297.933,1357.568);
   graph->SetPoint(40,1291.745,1376.546);
   graph->SetPoint(41,1285.962,1395.85);
   graph->SetPoint(42,1280.414,1415.373);
   graph->SetPoint(43,1275.052,1435.062);
   graph->SetPoint(44,1269.903,1455.034);
   graph->SetPoint(45,1265.021,1475.186);
   graph->SetPoint(46,1260.24,1495.442);
   graph->SetPoint(47,1255.567,1515.808);
   graph->SetPoint(48,1251.07,1536.417);
   graph->SetPoint(49,1246.716,1556.995);
   graph->SetPoint(50,1242.231,1577.512);
   graph->SetPoint(51,1237.757,1598.044);
   graph->SetPoint(52,1233.369,1618.736);
   graph->SetPoint(53,1229.137,1639.505);
   graph->SetPoint(54,1224.92,1660.301);
   graph->SetPoint(55,1220.819,1681.03);
   graph->SetPoint(56,1216.922,1701.533);
   graph->SetPoint(57,1213.104,1721.667);
   graph->SetPoint(58,1209.411,1741.348);
   graph->SetPoint(59,1205.887,1760.472);
   graph->SetPoint(60,1202.567,1778.919);
   graph->SetPoint(61,1199.479,1796.552);
   graph->SetPoint(62,1196.647,1813.234);
   graph->SetPoint(63,1194.15,1828.959);
   graph->SetPoint(64,1192.043,1843.502);
   graph->SetPoint(65,1190.166,1856.691);
   graph->SetPoint(66,1188.527,1868.469);
   
   return graph;
}

TGraph* Exp_7TeV_Bino2j_squark_gluino()
{
   TGraph *   graph = new TGraph(73);
   graph->SetName("Expected limit");
   graph->SetTitle("Expected limit");
   graph->SetFillColor(84);

   graph->SetLineColor(kBlack);
   graph->SetLineStyle(9);
   graph->SetLineWidth(3);
   graph->SetPoint(0,1848.383,958.3478);
   graph->SetPoint(1,1836.309,959.6609);
   graph->SetPoint(2,1822.728,961.108);
   graph->SetPoint(3,1807.765,962.8416);
   graph->SetPoint(4,1791.608,964.7966);
   graph->SetPoint(5,1774.235,966.8896);
   graph->SetPoint(6,1755.771,969.0971);
   graph->SetPoint(7,1736.438,971.5734);
   graph->SetPoint(8,1716.494,974.2629);
   graph->SetPoint(9,1695.959,977.0975);
   graph->SetPoint(10,1674.973,980.0726);
   graph->SetPoint(11,1653.745,983.3613);
   graph->SetPoint(12,1632.499,986.923);
   graph->SetPoint(13,1611.199,990.6977);
   graph->SetPoint(14,1590.197,994.8588);
   graph->SetPoint(15,1569.602,999.3593);
   graph->SetPoint(16,1549.278,1004.126);
   graph->SetPoint(17,1529.294,1009.315);
   graph->SetPoint(18,1509.677,1014.73);
   graph->SetPoint(19,1490.308,1020.56);
   graph->SetPoint(20,1471.522,1026.983);
   graph->SetPoint(21,1453.267,1033.86);
   graph->SetPoint(22,1435.467,1041.271);
   graph->SetPoint(23,1418.29,1049.321);
   graph->SetPoint(24,1401.787,1058.064);
   graph->SetPoint(25,1386.012,1067.557);
   graph->SetPoint(26,1370.945,1077.688);
   graph->SetPoint(27,1356.527,1088.552);
   graph->SetPoint(28,1343.052,1100.042);
   graph->SetPoint(29,1330.481,1112.178);
   graph->SetPoint(30,1318.964,1125.171);
   graph->SetPoint(31,1308.528,1139.008);
   graph->SetPoint(32,1299.037,1153.359);
   graph->SetPoint(33,1290.325,1168.328);
   graph->SetPoint(34,1282.652,1183.998);
   graph->SetPoint(35,1275.871,1200.125);
   graph->SetPoint(36,1269.722,1216.549);
   graph->SetPoint(37,1264.186,1233.367);
   graph->SetPoint(38,1259.316,1250.594);
   graph->SetPoint(39,1254.926,1268.045);
   graph->SetPoint(40,1250.852,1285.792);
   graph->SetPoint(41,1247.067,1303.713);
   graph->SetPoint(42,1243.288,1321.703);
   graph->SetPoint(43,1239.504,1339.921);
   graph->SetPoint(44,1235.75,1358.314);
   graph->SetPoint(45,1231.772,1376.68);
   graph->SetPoint(46,1227.538,1395.239);
   graph->SetPoint(47,1223.246,1413.981);
   graph->SetPoint(48,1218.758,1432.86);
   graph->SetPoint(49,1214.038,1451.726);
   graph->SetPoint(50,1209.016,1470.636);
   graph->SetPoint(51,1203.922,1489.785);
   graph->SetPoint(52,1198.841,1509.066);
   graph->SetPoint(53,1193.517,1528.474);
   graph->SetPoint(54,1188.023,1547.897);
   graph->SetPoint(55,1182.36,1567.226);
   graph->SetPoint(56,1176.683,1586.55);
   graph->SetPoint(57,1171.081,1606.025);
   graph->SetPoint(58,1165.631,1625.578);
   graph->SetPoint(59,1160.206,1645.172);
   graph->SetPoint(60,1154.917,1664.989);
   graph->SetPoint(61,1149.866,1684.707);
   graph->SetPoint(62,1144.937,1704.215);
   graph->SetPoint(63,1140.192,1723.46);
   graph->SetPoint(64,1135.685,1742.366);
   graph->SetPoint(65,1131.462,1760.833);
   graph->SetPoint(66,1127.624,1778.87);
   graph->SetPoint(67,1124.248,1796.234);
   graph->SetPoint(68,1121.187,1812.71);
   graph->SetPoint(69,1118.464,1828.166);
   graph->SetPoint(70,1116.148,1842.606);
   graph->SetPoint(71,1114.283,1855.821);
   graph->SetPoint(72,1112.691,1867.658);
   return graph;
}   
TGraph* Obs_7TeV_Wino2j_squark_gluino()
{
   TGraph *graph = new TGraph(96);
   graph->SetName("Observed limit");
   graph->SetTitle("Observed limit");
   graph->SetFillColor(84);

   graph->SetLineColor(kBlack);
   graph->SetLineWidth(2);
   graph->SetPoint(0,1865.486,813.3818);
   graph->SetPoint(1,1856.61,811.5335);
   graph->SetPoint(2,1846.409,809.8547);
   graph->SetPoint(3,1834.878,808.4351);
   graph->SetPoint(4,1822.217,807.268);
   graph->SetPoint(5,1808.442,806.2943);
   graph->SetPoint(6,1793.573,805.665);
   graph->SetPoint(7,1777.707,805.3736);
   graph->SetPoint(8,1760.948,805.3871);
   graph->SetPoint(9,1743.397,805.6492);
   graph->SetPoint(10,1725.148,806.0867);
   graph->SetPoint(11,1706.368,806.7966);
   graph->SetPoint(12,1687.243,807.666);
   graph->SetPoint(13,1667.709,808.5769);
   graph->SetPoint(14,1647.88,809.5522);
   graph->SetPoint(15,1627.841,810.6181);
   graph->SetPoint(16,1607.695,811.5826);
   graph->SetPoint(17,1587.488,812.3954);
   graph->SetPoint(18,1567.223,813.0348);
   graph->SetPoint(19,1546.886,813.5266);
   graph->SetPoint(20,1526.403,813.7611);
   graph->SetPoint(21,1505.805,814.0337);
   graph->SetPoint(22,1485.367,814.3888);
   graph->SetPoint(23,1465.039,814.9441);
   graph->SetPoint(24,1444.932,815.669);
   graph->SetPoint(25,1424.859,816.3897);
   graph->SetPoint(26,1404.859,817.4107);
   graph->SetPoint(27,1385.208,818.7954);
   graph->SetPoint(28,1365.852,820.6874);
   graph->SetPoint(29,1346.766,823.145);
   graph->SetPoint(30,1327.917,826.1912);
   graph->SetPoint(31,1309.445,829.8653);
   graph->SetPoint(32,1291.516,834.3318);
   graph->SetPoint(33,1274.088,839.5042);
   graph->SetPoint(34,1257.088,845.494);
   graph->SetPoint(35,1240.684,852.4135);
   graph->SetPoint(36,1224.767,859.9914);
   graph->SetPoint(37,1209.213,868.3911);
   graph->SetPoint(38,1194.26,877.5719);
   graph->SetPoint(39,1179.806,887.5518);
   graph->SetPoint(40,1165.989,898.3644);
   graph->SetPoint(41,1152.601,909.869);
   graph->SetPoint(42,1139.639,922.1261);
   graph->SetPoint(43,1127.304,935.0054);
   graph->SetPoint(44,1115.497,948.3185);
   graph->SetPoint(45,1104.092,962.0923);
   graph->SetPoint(46,1093.136,976.243);
   graph->SetPoint(47,1082.566,990.8642);
   graph->SetPoint(48,1072.478,1005.894);
   graph->SetPoint(49,1062.81,1021.428);
   graph->SetPoint(50,1053.635,1037.361);
   graph->SetPoint(51,1044.765,1053.592);
   graph->SetPoint(52,1036.18,1070.1);
   graph->SetPoint(53,1027.852,1086.854);
   graph->SetPoint(54,1019.832,1103.992);
   graph->SetPoint(55,1012.255,1121.56);
   graph->SetPoint(56,1005.024,1139.397);
   graph->SetPoint(57,998.0006,1157.453);
   graph->SetPoint(58,991.1918,1175.709);
   graph->SetPoint(59,984.5508,1194.114);
   graph->SetPoint(60,978.0493,1212.665);
   graph->SetPoint(61,971.7679,1231.509);
   graph->SetPoint(62,965.6857,1250.38);
   graph->SetPoint(63,959.5999,1269.387);
   graph->SetPoint(64,953.7057,1288.505);
   graph->SetPoint(65,947.8571,1307.679);
   graph->SetPoint(66,942.1485,1327.072);
   graph->SetPoint(67,936.5729,1346.429);
   graph->SetPoint(68,930.8761,1365.742);
   graph->SetPoint(69,925.2771,1385.227);
   graph->SetPoint(70,920.027,1404.718);
   graph->SetPoint(71,914.9547,1424.043);
   graph->SetPoint(72,910.0528,1443.352);
   graph->SetPoint(73,905.3765,1462.661);
   graph->SetPoint(74,900.9795,1481.99);
   graph->SetPoint(75,896.9751,1501.493);
   graph->SetPoint(76,893.4573,1521.091);
   graph->SetPoint(77,890.3628,1540.877);
   graph->SetPoint(78,887.7793,1560.795);
   graph->SetPoint(79,885.4805,1580.648);
   graph->SetPoint(80,883.3638,1600.531);
   graph->SetPoint(81,881.5404,1620.549);
   graph->SetPoint(82,879.9726,1640.738);
   graph->SetPoint(83,878.6716,1661.254);
   graph->SetPoint(84,877.6256,1681.748);
   graph->SetPoint(85,876.6091,1702.072);
   graph->SetPoint(86,875.5774,1722.135);
   graph->SetPoint(87,874.5564,1741.954);
   graph->SetPoint(88,873.5694,1761.287);
   graph->SetPoint(89,872.4428,1779.911);
   graph->SetPoint(90,871.1941,1797.683);
   graph->SetPoint(91,869.8524,1814.455);
   graph->SetPoint(92,868.5179,1830.221);
   graph->SetPoint(93,867.2811,1844.754);
   graph->SetPoint(94,866.0213,1857.891);
   graph->SetPoint(95,864.6609,1869.644);
   

  return graph;
}

TGraph* Exp_7TeV_Wino2j_squark_gluino()
{
   TGraph * graph = new TGraph(101);
   graph->SetName("Expected limit");
   graph->SetTitle("Expected limit");
   graph->SetFillColor(84);

   graph->SetLineColor(kBlack);
   graph->SetLineStyle(9);
   graph->SetLineWidth(3);
   graph->SetPoint(0,1857.635,720.5431);
   graph->SetPoint(1,1847.236,719.5941);
   graph->SetPoint(2,1835.531,718.8794);
   graph->SetPoint(3,1822.676,718.383);
   graph->SetPoint(4,1808.622,718.0646);
   graph->SetPoint(5,1793.475,717.9358);
   graph->SetPoint(6,1777.352,717.9959);
   graph->SetPoint(7,1760.461,718.412);
   graph->SetPoint(8,1743.042,719.1325);
   graph->SetPoint(9,1725.085,720.0858);
   graph->SetPoint(10,1706.692,721.2602);
   graph->SetPoint(11,1687.94,722.6418);
   graph->SetPoint(12,1669.061,724.1284);
   graph->SetPoint(13,1650.053,725.5806);
   graph->SetPoint(14,1630.94,727.152);
   graph->SetPoint(15,1611.78,728.8469);
   graph->SetPoint(16,1592.682,730.6328);
   graph->SetPoint(17,1573.709,732.4408);
   graph->SetPoint(18,1554.693,734.1867);
   graph->SetPoint(19,1535.603,735.8502);
   graph->SetPoint(20,1516.348,737.2854);
   graph->SetPoint(21,1496.855,738.5872);
   graph->SetPoint(22,1477.346,740.0043);
   graph->SetPoint(23,1457.906,741.4989);
   graph->SetPoint(24,1438.382,743.0287);
   graph->SetPoint(25,1418.627,744.684);
   graph->SetPoint(26,1398.81,746.7202);
   graph->SetPoint(27,1379.042,748.8377);
   graph->SetPoint(28,1359.129,751.1258);
   graph->SetPoint(29,1339.306,753.8679);
   graph->SetPoint(30,1319.668,757.0353);
   graph->SetPoint(31,1300.165,760.7473);
   graph->SetPoint(32,1280.848,764.8294);
   graph->SetPoint(33,1261.635,769.49);
   graph->SetPoint(34,1242.812,774.7414);
   graph->SetPoint(35,1224.336,780.661);
   graph->SetPoint(36,1206.407,787.3456);
   graph->SetPoint(37,1188.881,794.7183);
   graph->SetPoint(38,1171.819,802.9031);
   graph->SetPoint(39,1155.483,811.8295);
   graph->SetPoint(40,1139.832,821.3644);
   graph->SetPoint(41,1124.726,831.4531);
   graph->SetPoint(42,1110.183,842.1078);
   graph->SetPoint(43,1096.268,853.4511);
   graph->SetPoint(44,1083.094,865.5313);
   graph->SetPoint(45,1070.56,878.1542);
   graph->SetPoint(46,1058.531,891.3365);
   graph->SetPoint(47,1047.04,904.9813);
   graph->SetPoint(48,1035.926,918.9885);
   graph->SetPoint(49,1025.209,933.4418);
   graph->SetPoint(50,1014.971,948.3624);
   graph->SetPoint(51,1005.118,963.591);
   graph->SetPoint(52,995.4922,979.0378);
   graph->SetPoint(53,986.1289,994.8037);
   graph->SetPoint(54,977.0703,1010.802);
   graph->SetPoint(55,968.2573,1027.132);
   graph->SetPoint(56,959.7054,1043.563);
   graph->SetPoint(57,951.2446,1060.235);
   graph->SetPoint(58,943.0975,1077.147);
   graph->SetPoint(59,935.2026,1094.397);
   graph->SetPoint(60,927.5693,1111.744);
   graph->SetPoint(61,919.9541,1129.188);
   graph->SetPoint(62,912.5812,1146.95);
   graph->SetPoint(63,905.5237,1164.949);
   graph->SetPoint(64,898.6385,1183.131);
   graph->SetPoint(65,892.0202,1201.657);
   graph->SetPoint(66,885.7465,1220.452);
   graph->SetPoint(67,879.7459,1239.601);
   graph->SetPoint(68,874.0224,1258.863);
   graph->SetPoint(69,868.3313,1278.236);
   graph->SetPoint(70,862.898,1297.944);
   graph->SetPoint(71,857.7971,1317.905);
   graph->SetPoint(72,852.885,1338.065);
   graph->SetPoint(73,848.2513,1358.578);
   graph->SetPoint(74,843.9635,1379.355);
   graph->SetPoint(75,840.0494,1400.249);
   graph->SetPoint(76,836.4982,1421.154);
   graph->SetPoint(77,833.3428,1442.326);
   graph->SetPoint(78,830.6605,1463.65);
   graph->SetPoint(79,828.2255,1484.865);
   graph->SetPoint(80,825.9482,1506.004);
   graph->SetPoint(81,824.0259,1527.245);
   graph->SetPoint(82,822.4131,1548.294);
   graph->SetPoint(83,820.806,1569.102);
   graph->SetPoint(84,819.2994,1589.729);
   graph->SetPoint(85,817.9092,1610.306);
   graph->SetPoint(86,816.6293,1630.741);
   graph->SetPoint(87,815.2405,1650.984);
   graph->SetPoint(88,813.7052,1671.084);
   graph->SetPoint(89,811.9948,1690.833);
   graph->SetPoint(90,810.0949,1710.207);
   graph->SetPoint(91,808.0092,1729.171);
   graph->SetPoint(92,805.7607,1747.672);
   graph->SetPoint(93,803.4527,1765.77);
   graph->SetPoint(94,801.1879,1783.273);
   graph->SetPoint(95,798.8659,1800.006);
   graph->SetPoint(96,796.5647,1815.868);
   graph->SetPoint(97,794.3557,1830.748);
   graph->SetPoint(98,792.2986,1844.54);
   graph->SetPoint(99,790.4383,1857.148);
   graph->SetPoint(100,788.673,1868.557);
   return graph;
}   
