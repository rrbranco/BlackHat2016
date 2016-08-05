#pragma once

#include <memory.h>
#include <ctype.h>
#include "VisualDataTracer.h"
#include "RangeAddWindow.h"
#include "AboutWindow.h"
#include "AnalysisResultsWindow.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strncpy


#define VSCROLL_WIDTH 16 // TODO: fix this

extern bool vdt_file_ready;
extern bool is_instr_tainted;
extern bool closeWindowAfterAnalysis;
extern int instr_index_analyze;

namespace VisualDataTracer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Threading;

	/// <summary>
	/// Summary for MainWindow
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class MainWindow : public System::Windows::Forms::Form
	{
	private: int loadedInstrs;
	private: System::String ^VDT_FileName;
	private: System::String ^ rangeElements;
	public: Hashtable ^testHash;
	public:
		void scrollToItem(int idx)
		{
			try
			{
				this->listBox1->SelectedIndex = idx - 1;
			}
			catch (Exception ^e)
			{
				(void)e;
				MessageBox::Show("Invalid index!", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		}
		void scrollToStart()
		{
			scrollToItem(1);
		}
		void scrollToEnd()
		{
			scrollToItem(loadedInstrs);
		}

	private:
		void FileParse(System::String ^FileName)
		{
			VDTAnalyzer::GlobalInstrMap = gcnew Hashtable();
			VDTAnalyzer::GlobalTaintRanges = gcnew Hashtable();

			array<unsigned char> ^magic = gcnew array<unsigned char>(sizeof(VDT_FILE_MAGIC));

			array<unsigned char> ^traceEntry = gcnew array<unsigned char>(VDT_ENTRY_SIZE);
			pin_ptr<unsigned char> tmpPtr2 = &traceEntry[0];

			unsigned int totalInstrs = 0;
			unsigned long long instrIdx = 0;
			FileStream ^vdtFile;


			if (vdt_file_ready)
			{
				vdtFile = File::Open(FileName, System::IO::FileMode::Open);
			} 
			else
			{
				vdtFile = File::Open(this->openFileDialog1->FileName, System::IO::FileMode::Open);
			}

			if (!vdtFile->Read(magic, 0, sizeof(VDT_FILE_MAGIC)))
				return;

			/* casting out the integer convertion loss warning since it is not a problem here */
			totalInstrs = (unsigned int) (vdtFile->Length - sizeof(VDT_FILE_MAGIC))/VDT_ENTRY_SIZE;

			for (;vdtFile->Read(traceEntry, 0, VDT_ENTRY_SIZE); instrIdx++)
			{
				VDTAnalyzer::GlobalInstrMap[instrIdx] = gcnew VdtInstruction((char *)tmpPtr2);

				if (instrIdx % 1000)
					this->backgroundWorker1->ReportProgress((int)((instrIdx*100)/totalInstrs));

				//if (instrIdx > 100)
				//	break;
			}

			this->loadedInstrs = totalInstrs;

			vdtFile->Close();
		}

	public:
		/* rangeElements ->  rangeStart-rangeEnd,range2Start-rang2End */
		MainWindow(System::String ^ FileName, System::String ^ rangeElements)
		{
			InitializeComponent();
			

			// Debug only (until implemented the OutputDebug() function
			//printf("\nhere MainWindow()\n");

			this->Show();

			this->loadedInstrs = 0;

			this->listBox1->Width = this->Width - VSCROLL_WIDTH;
			this->listBox1->Height = this->statusStrip1->Top - this->menuStrip1->Bottom;

			this->Resize += gcnew EventHandler(this, &MainWindow::MainWindow_Resize);

			this->backgroundWorker1->WorkerReportsProgress = true;

			this->backgroundWorker1->ProgressChanged += gcnew ProgressChangedEventHandler(this, &MainWindow::backgroundWorker1_ProgressChanged);
			this->backgroundWorker1->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler(this, &MainWindow::backgroundWorker1_RunWorkerCompleted);

			this->statusStrip1->Items->Add(gcnew ToolStripStatusLabel("Ready"));			


			/* To automatically process the file */
			//if (this->openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			//{

				if (vdt_file_ready) {
					this->VDT_FileName = FileName;
					this->rangeElements = rangeElements;
					//System::Console::Write("file ready, reading it...");
					//System::Console::Write(FileName);


					array<unsigned char> ^magic = gcnew array<unsigned char>(sizeof(VDT_FILE_MAGIC));
					pin_ptr<unsigned char> tmpPtr = &magic[0];

					FileStream ^vdtFile = File::Open(FileName, System::IO::FileMode::Open);

					if (!vdtFile->Read(magic, 0, sizeof(VDT_FILE_MAGIC)) || memcmp(tmpPtr, VDT_FILE_MAGIC, sizeof(VDT_FILE_MAGIC)))
					{
						MessageBox::Show("Invalid file!", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
						System::Console::Write("Invalid File!");
						vdtFile->Close();
						return;
					}

					ToolStripProgressBar ^bar = gcnew ToolStripProgressBar();
					bar->Maximum = 100;
					bar->Minimum = 0;

					this->statusStrip1->Items->Add(bar);
					this->statusStrip1->Items[0]->Text = "Processing file...";
					System::Console::Write("Processing file...");

					vdtFile->Close();
			
					this->backgroundWorker1->RunWorkerAsync();

				
				}
			//}
			/* end of automatic file processing */
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainWindow()
		{
			if (components)
			{
				delete components;
			}

			if (this->backgroundWorker1->IsBusy)
				this->backgroundWorker1->CancelAsync();
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	protected: 
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;

	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::ListBox^  listBox1;
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  aToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  addTaintRangeToolStripMenuItem;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  checkTaintOfToolStripMenuItem;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
	private: System::Windows::Forms::ToolStripMenuItem^  scrollToItemToolStripMenuItem;
	private: System::ComponentModel::IContainer^  components;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainWindow::typeid));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addTaintRangeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->checkTaintOfToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->scrollToItemToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->menuStrip1->SuspendLayout();
			this->contextMenuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->fileToolStripMenuItem, 
				this->aToolStripMenuItem, this->helpToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(969, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->openToolStripMenuItem, 
				this->exitToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::O));
			this->openToolStripMenuItem->Size = System::Drawing::Size(146, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWindow::openToolStripMenuItem_Click);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Q));
			this->exitToolStripMenuItem->Size = System::Drawing::Size(146, 22);
			this->exitToolStripMenuItem->Text = L"Exit";
			// 
			// aToolStripMenuItem
			// 
			this->aToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->addTaintRangeToolStripMenuItem});
			this->aToolStripMenuItem->Name = L"aToolStripMenuItem";
			this->aToolStripMenuItem->Size = System::Drawing::Size(62, 20);
			this->aToolStripMenuItem->Text = L"Analysis";
			// 
			// addTaintRangeToolStripMenuItem
			// 
			this->addTaintRangeToolStripMenuItem->Name = L"addTaintRangeToolStripMenuItem";
			this->addTaintRangeToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::R));
			this->addTaintRangeToolStripMenuItem->Size = System::Drawing::Size(203, 22);
			this->addTaintRangeToolStripMenuItem->Text = L"Add Taint Range";
			this->addTaintRangeToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWindow::addTaintRangeToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->aboutToolStripMenuItem});
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->helpToolStripMenuItem->Text = L"Help";
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(107, 22);
			this->aboutToolStripMenuItem->Text = L"About";
			this->aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWindow::aboutToolStripMenuItem_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->DefaultExt = L"vdt";
			this->openFileDialog1->Filter = L"Visual Data Trace files|*.vdt|All Files|*.*";
			this->openFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MainWindow::openFileDialog1_FileOk);
			// 
			// listBox1
			// 
			this->listBox1->ContextMenuStrip = this->contextMenuStrip1;
			this->listBox1->FormattingEnabled = true;
			this->listBox1->Location = System::Drawing::Point(0, 27);
			this->listBox1->Name = L"listBox1";
			this->listBox1->ScrollAlwaysVisible = true;
			this->listBox1->Size = System::Drawing::Size(288, 498);
			this->listBox1->TabIndex = 1;
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->checkTaintOfToolStripMenuItem, 
				this->scrollToItemToolStripMenuItem});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(154, 70);
			this->contextMenuStrip1->Opening += gcnew System::ComponentModel::CancelEventHandler(this, &MainWindow::contextMenuStrip1_Opening);
			// 
			// checkTaintOfToolStripMenuItem
			// 
			this->checkTaintOfToolStripMenuItem->Name = L"checkTaintOfToolStripMenuItem";
			this->checkTaintOfToolStripMenuItem->Size = System::Drawing::Size(153, 22);
			this->checkTaintOfToolStripMenuItem->Text = L"Check Taint Of";
			// 
			// scrollToItemToolStripMenuItem
			// 
			this->scrollToItemToolStripMenuItem->Name = L"scrollToItemToolStripMenuItem";
			this->scrollToItemToolStripMenuItem->Size = System::Drawing::Size(153, 22);
			this->scrollToItemToolStripMenuItem->Text = L"Scroll To Item";
			this->scrollToItemToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWindow::scrollToItemToolStripMenuItem_Click);
			// 
			// statusStrip1
			// 
			this->statusStrip1->Location = System::Drawing::Point(0, 529);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(969, 22);
			this->statusStrip1->TabIndex = 2;
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainWindow::backgroundWorker1_DoWork);
			// 
			// MainWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(969, 551);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->listBox1);
			this->Controls->Add(this->menuStrip1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MainWindow";
			this->Text = L"Visual Data Tracer";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Load += gcnew System::EventHandler(this, &MainWindow::Form1_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->contextMenuStrip1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (this->openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			 {
				 array<unsigned char> ^magic = gcnew array<unsigned char>(sizeof(VDT_FILE_MAGIC));
			     pin_ptr<unsigned char> tmpPtr = &magic[0];

				 FileStream ^vdtFile = File::Open(this->openFileDialog1->FileName, System::IO::FileMode::Open);

				 if (!vdtFile->Read(magic, 0, sizeof(VDT_FILE_MAGIC)) || memcmp(tmpPtr, VDT_FILE_MAGIC, sizeof(VDT_FILE_MAGIC)))
				 {
					 MessageBox::Show("Invalid file!", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
					 vdtFile->Close();
					 return;
				 }

				 ToolStripProgressBar ^bar = gcnew ToolStripProgressBar();
				 bar->Maximum = 100;
				 bar->Minimum = 0;

				 this->statusStrip1->Items->Add(bar);
				 this->statusStrip1->Items[0]->Text = "Processing file...";

				 vdtFile->Close();
				 
				 this->backgroundWorker1->RunWorkerAsync();
			 }			 
		 }
private: System::Void openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
		 }
private: System::Void addTaintRangeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 RangeAddWindow ^rangeAdd = gcnew RangeAddWindow();
			 rangeAdd->Show();
		 }

private: System::Void checkTaintOfItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 ArrayList ^result = VDTAnalyzer::SearchTaintOf(((ToolStripButton ^)sender)->Text, this->listBox1->SelectedIndex);

			 if (result->Count == 0)
			 {
				 MessageBox::Show("No source of taint found!", "Analysis Results", MessageBoxButtons::OK, MessageBoxIcon::Information);
			 }
			 else
			 {
				AnalysisResultsWindow ^resultsWin = gcnew AnalysisResultsWindow();
				resultsWin->SetResult(result);
				resultsWin->Show();
			 }
		 }
private: System::Void contextMenuStrip1_Opening(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
			 if (this->backgroundWorker1->IsBusy)
				 return;

			 unsigned long long index = this->listBox1->SelectedIndex;

			 this->checkTaintOfToolStripMenuItem->Enabled = false;

			 this->scrollToItemToolStripMenuItem->Enabled = false;
			 if (this->loadedInstrs > 0)
				 this->scrollToItemToolStripMenuItem->Enabled = true;

			 if (index == ULLONG_MAX)
				 return;

			 VdtInstruction ^instr = (VdtInstruction ^) VDTAnalyzer::GlobalInstrMap[index];

			 if (instr == nullptr)  // should never happen since index is supposed to be valid
				 return;

			 System::Console::Write("Instruction index choosen: " + index);
			 System::Console::Write("Instruction fulldump: " + instr->FullDump);

			 this->checkTaintOfToolStripMenuItem->DropDownItems->Clear();
			 
			 if (!String::IsNullOrEmpty(instr->Src) && !isdigit(instr->Src[0]))
			 {
				ToolStripButton ^item = gcnew ToolStripButton(instr->Src);				
				item->Click += gcnew System::EventHandler(this, &MainWindow::checkTaintOfItem_Click);

				this->checkTaintOfToolStripMenuItem->DropDownItems->Add(item);
				this->checkTaintOfToolStripMenuItem->Enabled = true;
			 }
			 if (!String::IsNullOrEmpty(instr->SrcDep1) && !isdigit(instr->SrcDep1[0]))
			 {
				ToolStripButton ^item = gcnew ToolStripButton(instr->SrcDep1);
				item->Click += gcnew System::EventHandler(this, &MainWindow::checkTaintOfItem_Click);

				this->checkTaintOfToolStripMenuItem->DropDownItems->Add(item);
				this->checkTaintOfToolStripMenuItem->Enabled = true;
			 }
			 if (!String::IsNullOrEmpty(instr->SrcDep2) && !isdigit(instr->SrcDep2[0]))
			 {
				ToolStripButton ^item = gcnew ToolStripButton(instr->SrcDep2);
				item->Click += gcnew System::EventHandler(this, &MainWindow::checkTaintOfItem_Click);

				this->checkTaintOfToolStripMenuItem->DropDownItems->Add(item);
				this->checkTaintOfToolStripMenuItem->Enabled = true;
			 }
			 if (!String::IsNullOrEmpty(instr->SrcDep3) && !isdigit(instr->SrcDep3[0]))
			 {
				ToolStripButton ^item = gcnew ToolStripButton(instr->SrcDep3);
				item->Click += gcnew System::EventHandler(this, &MainWindow::checkTaintOfItem_Click);

				this->checkTaintOfToolStripMenuItem->DropDownItems->Add(item);
				this->checkTaintOfToolStripMenuItem->Enabled = true;
			 }
		 }
private: System::Void aboutToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 AboutWindow ^about = gcnew AboutWindow();
			 about->Show();
		 }

private: System::Void MainWindow_Resize(System::Object^  sender, EventArgs^  e) {
			 this->listBox1->Width = this->Width - VSCROLL_WIDTH;
			 this->listBox1->Height = this->statusStrip1->Top - this->menuStrip1->Bottom;
         }

private: System::Void backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
			 if (vdt_file_ready)
			  	this->FileParse(this->VDT_FileName);	
			 else
			 	this->FileParse(nullptr);

		 }

private: System::Void backgroundWorker1_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e) {
			 ((ToolStripProgressBar^)this->statusStrip1->Items[1])->Value = e->ProgressPercentage;
		 }

private: System::Void backgroundWorker1_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) {
			 
			 this->statusStrip1->Items[0]->Text = "Building display list... This may take a while!";
			 this->statusStrip1->Items->RemoveAt(1);
			 this->statusStrip1->Refresh();

			 unsigned long long i = 0;
			 VdtInstruction ^instr;

			 this->listBox1->BeginUpdate();
			 while ((instr = (VdtInstruction ^) VDTAnalyzer::GlobalInstrMap[i++]) != nullptr)
			 {
				 this->listBox1->Items->Add(String::Concat(String::Concat(i, ".\t\t"), instr->FullDump));
			 }

			 this->listBox1->EndUpdate();
			 

			 this->statusStrip1->Items[0]->Text = "Done!";
			 this->statusStrip1->Refresh();

			 if (vdt_file_ready)
			 {
				 unsigned long long instrToCheckTaint;

				 /* if passed as parameter */
				 if (instr_index_analyze > -1) {
					 instrToCheckTaint = (unsigned long long) instr_index_analyze;
				 }
				 else {
					 // TODO: the -1 is just for the tests, it should be the latest one by default
					 //instrToCheckTaint = (this->loadedInstrs) - 1;  // previous to last because of our sample, use last later (or the one defined)
					 instrToCheckTaint = (this->loadedInstrs);
				 }
				 System::Console::Write("\nNumber of instrs (and instruction to check taint of): " + this->loadedInstrs + " " + instrToCheckTaint);
				
				 //System::Console::Write("Range: " + ((String ^)this->rangeElements));


				 array<String^>^ range_values = this->rangeElements->Split(',');

				 for (int ji=0; ji < range_values->Length; ji++)
				 {
					 //System::Console::Write("Range " + ji + " : " + range_values[ji]);
					 array<String^>^ range_values_final = range_values[ji]->Split('-');
					 
					 System::Console::Write("\nRange Start: " + range_values_final[0] + " Range End: " + range_values_final[1]);

					 // add the checks for the ranges? not sure if worth it (the GUI has it because makes more sense)
					 VDTAnalyzer::GlobalTaintRanges->Add(range_values_final[0], range_values_final[1]);
					 
				 }


				 IEnumerator ^keyEnum = VDTAnalyzer::GlobalTaintRanges->Keys->GetEnumerator();

				 /*
				 // in case you want to make sure the command line received start ranges are correctly set
				 while (keyEnum->MoveNext())
				 {
					 // print the ranges for the taint analysis
					 System::Console::Write("\nControlled Start Ranges: " + ((String ^)keyEnum->Current)->ToLower() + "\n"); 
				 }*/

				 VdtInstruction ^instr = (VdtInstruction ^)VDTAnalyzer::GlobalInstrMap[--instrToCheckTaint]; // starts from 0 (therefore, the --)

				 if (instr != nullptr)
					 System::Console::Write("\nInstruction: " + instr->FullDump + "\n");
				 else
					 System::Console::Write("\nSomething went wrong! No instruction found?");
				 
				 /* full instruction dump to the console */
				 /*
				 //unsigned long long i = 0;  // use 0 for complete list
				 unsigned long long i = 124110; // for our development test, that is close to the end
				 //VdtInstruction ^instr;

				 while ((instr = (VdtInstruction ^)VDTAnalyzer::GlobalInstrMap[i++]) != nullptr)
				 {
					 System::Console::Write("Index " + i + ": " + instr->FullDump);
				 }
				 */
				 /* end of full instruction dump to the console */

				 System::Console::Write("\nDumping instruction taint information:\n\n");
				 
				 if (!String::IsNullOrEmpty(instr->Src) && !isdigit(instr->Src[0]))
				 {
					 System::Console::Write("instr->Src tainted: " + instr->Src + "\n");
					 is_instr_tainted = 1;
				 }
				 if (!String::IsNullOrEmpty(instr->SrcDep1) && !isdigit(instr->SrcDep1[0]))
				 {
					 System::Console::Write("instr->SrcDep1 tainted: " + instr->SrcDep1 + "\n");
					 is_instr_tainted = 1; 
				 }
				 if (!String::IsNullOrEmpty(instr->SrcDep2) && !isdigit(instr->SrcDep2[0]))
				 {
					 System::Console::Write("instr->SrcDep1 tainted: " + instr->SrcDep2 + "\n");
					 is_instr_tainted = 1;
				 }
				 if (!String::IsNullOrEmpty(instr->SrcDep3) && !isdigit(instr->SrcDep3[0]))
				 {
					 System::Console::Write("instr->SrcDep1 tainted: " + instr->SrcDep3 + "\n");
					 is_instr_tainted = 1;
				 }
				 
				 /* After analysis is done, close the APP if the cmdline says so (used for integrating with forward analysis) */
				 if (closeWindowAfterAnalysis) {
					 System::Console::Write("\nClosing GUI \n");
					 exit(EXIT_SUCCESS);
				 }
				 else {
					 System::Console::Write("\nNOT Closing GUI \n");
				 }



			 }
		 }
private: System::Void scrollToItemToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
};
}

