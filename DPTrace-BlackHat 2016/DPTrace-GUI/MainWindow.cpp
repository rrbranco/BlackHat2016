#include "StdAfx.h"
#include "MainWindow.h"

//Using this just to solve the recursive inclusion issue with ScrollToItemWindow
#include "ScrollToItemWindow.h"

using namespace VisualDataTracer;

System::Void MainWindow::scrollToItemToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	
	ScrollToItemWindow ^scrollToItem = gcnew ScrollToItemWindow((MainWindow^) this);
	scrollToItem->Show();

}

