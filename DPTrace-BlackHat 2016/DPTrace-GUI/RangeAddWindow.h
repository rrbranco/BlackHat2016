#pragma once

#include "VisualDataTracer.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Text::RegularExpressions;


namespace VisualDataTracer {

	/// <summary>
	/// Summary for RangeAddWindow
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class RangeAddWindow : public System::Windows::Forms::Form
	{
	public:
		RangeAddWindow(void)
		{
			InitializeComponent();
			
			this->listView1->BeginUpdate();

			IEnumerator ^keyEnum = VDTAnalyzer::GlobalTaintRanges->Keys->GetEnumerator();

			while (keyEnum->MoveNext())
			{
				ListViewItem ^item = gcnew ListViewItem(String::Concat("0x", ((String ^) keyEnum->Current)->ToLower()));
				item->SubItems->Add(String::Concat("0x", ((String ^)VDTAnalyzer::GlobalTaintRanges[keyEnum->Current]))->ToLower());

				this->listView1->Items->Add(item);
			}

			this->listView1->EndUpdate();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RangeAddWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::ListView^  listView1;
	private: System::Windows::Forms::ColumnHeader^  Start;
	private: System::Windows::Forms::ColumnHeader^  End;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;




	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->listView1 = (gcnew System::Windows::Forms::ListView());
			this->Start = (gcnew System::Windows::Forms::ColumnHeader());
			this->End = (gcnew System::Windows::Forms::ColumnHeader());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 32);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(29, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Start";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(101, 32);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(26, 13);
			this->label2->TabIndex = 1;
			this->label2->Text = L"End";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(15, 49);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(73, 20);
			this->textBox1->TabIndex = 2;
			this->textBox1->Text = L"0x00000000";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(104, 49);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(73, 20);
			this->textBox2->TabIndex = 3;
			this->textBox2->Text = L"0x00000000";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(197, 47);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 4;
			this->button1->Text = L"Add";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &RangeAddWindow::button1_Click);
			// 
			// listView1
			// 
			this->listView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {this->Start, this->End});
			this->listView1->FullRowSelect = true;
			this->listView1->Location = System::Drawing::Point(15, 76);
			this->listView1->MultiSelect = false;
			this->listView1->Name = L"listView1";
			this->listView1->Size = System::Drawing::Size(162, 176);
			this->listView1->TabIndex = 5;
			this->listView1->UseCompatibleStateImageBehavior = false;
			this->listView1->View = System::Windows::Forms::View::Details;
			// 
			// Start
			// 
			this->Start->Text = L"Start";
			this->Start->Width = 81;
			// 
			// End
			// 
			this->End->Text = L"End";
			this->End->Width = 77;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(197, 76);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 6;
			this->button2->Text = L"Remove";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &RangeAddWindow::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(197, 229);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 7;
			this->button3->Text = L"Close";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &RangeAddWindow::button3_Click);
			// 
			// RangeAddWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 264);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->listView1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"RangeAddWindow";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Add Taint Range";
			this->TopMost = true;
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

				 String ^start = this->textBox1->Text;
				 String ^end = this->textBox2->Text;
				
				 Regex ^regex = gcnew Regex("^[a-fA-F0-9]+$");

				 if (start->Length != 10 || end->Length != 10 ||
					 !start->StartsWith("0x") || !end->StartsWith("0x") ||
					 !regex->IsMatch(start->Substring(2)) || !regex->IsMatch(end->Substring(2)))
				 {
					 MessageBox::Show("Addresses must in hexadecimal format, with 8 characters preceded by 0x", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
					 return;
				 }

				 if (start->CompareTo(end) >= 0)
				 {
					 MessageBox::Show("\'Start\' address must be lower than \'End\'", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
					 return;
				 }

				 if (VDTAnalyzer::GlobalTaintRanges->Contains(start->Substring(2)))
				 {
					 MessageBox::Show("There is already a range starting at that address", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
					 return;
				 }

				 this->listView1->BeginUpdate();

				 ListViewItem ^item = gcnew ListViewItem(start);
				 item->SubItems->Add(end);

				 this->listView1->Items->Add(item);

				 this->listView1->EndUpdate();

				 VDTAnalyzer::GlobalTaintRanges->Add(start->Substring(2), end->Substring(2));
			 }
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (this->listView1->SelectedItems->Count == 0)
				 return;

			 ListViewItem ^item = this->listView1->SelectedItems[0];
			 VDTAnalyzer::GlobalTaintRanges->Remove(item->Text);

			 this->listView1->Items->Remove(item);
		 }
};
}
