import root;
import pad_layout;

string topDir = "../../";

string dataset = "run_physics_no_margin/274244";

TH2_palette = Gradient(blue, heavygreen, yellow, red);

//----------------------------------------------------------------------------------------------------

string f = topDir + "/" + dataset + "/distributions.root";

NewPad("$x_{\rm L,210,N}\ung{mm}$", "$x_{\rm L,210,F}\ung{mm}$");
scale(Linear, Linear, Log);
draw(RootGetObject(f, "cuts/cut 1/canvas_before|h2_x_L_1_F_vs_x_L_1_N"));
draw(RootGetObject(f, "cuts/cut 1/canvas_before|g_up"), "l", black+1pt);
draw(RootGetObject(f, "cuts/cut 1/canvas_before|g_down"), "l", black+1pt);
limits((5, 5), (20, 20), Crop);

NewPad("$x_{\rm R,210,N}\ung{mm}$", "$x_{\rm R,210,F}\ung{mm}$");
scale(Linear, Linear, Log);
draw(RootGetObject(f, "cuts/cut 2/canvas_before|h2_x_R_1_F_vs_x_R_1_N"));
draw(RootGetObject(f, "cuts/cut 2/canvas_before|g_up"), "l", black+1pt);
draw(RootGetObject(f, "cuts/cut 2/canvas_before|g_down"), "l", black+1pt);
limits((5, 5), (20, 20), Crop);
