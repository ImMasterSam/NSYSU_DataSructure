import tkinter as tk
from tkinter import filedialog
from os import system, stat, path

class HuffmanGUI:

    # 建構式
    def __init__(self, root : tk.Tk):

        # 設定主視窗
        self.root = root
        self.root.resizable(True , True)
        self.root.minsize(450, 550)

        # 執行檔位置
        self.execute_path = path.dirname(path.realpath(__file__)).replace('\\', '/') + "/huffman.exe"
        self.operation = 1 # 1: Compression 0: Decompression
        
        # 設定視窗大小
        screen_width = root.winfo_screenwidth()
        screen_height = root.winfo_screenheight()
        window_width = screen_width // 2
        window_height = screen_height // 2
        
        # 將視窗置中
        x = (screen_width - window_width) // 2
        y = (screen_height - window_height) // 2
        self.root.geometry(f"{window_width}x{window_height}+{x}+{y}")
        
        # 標題欄
        self.title_bar = tk.Frame(root, bg = "#32393f", height = 60)
        self.title_bar.pack_propagate(False)
        self.title_bar.pack(side = "top", fill = "x", expand = True)

        # 標題欄 : 標題
        self.title_label = tk.Label(self.title_bar, text = "Huffman", padx = 30)
        self.title_label.configure(bg = "#32393f", fg = "#c7e9ff", font = ("Arial", 30, "bold"))
        self.title_label.pack(side = "left")

        # 主要內容
        self.content = tk.Frame(root, bg = "#575f64", height = screen_height - 60)
        self.content.pack_propagate(False)
        self.content.pack(fill = "both", expand = True)
        
        # 輸入檔區
        self.input_frame = tk.Frame(self.content, bg = "#474f54")
        self.input_frame.pack(fill = "x", padx = 20, pady = 10)
        self.input_label = tk.Label(self.input_frame, text = "Input File")
        self.input_label.configure(bg = "#474f54", fg = "#ffffff", font = ("Arial", 15, "bold"))
        self.input_label.pack(side = "top", pady = 5)
        self.input_path = tk.StringVar()
        self.input_entry = tk.Entry(self.input_frame, textvariable = self.input_path, font = ("Microsoft JhengHei", 12, "bold"))
        self.input_entry.pack(side = "top", fill = "x", expand = True, padx = 10, pady = 10)
        self.input_button = tk.Button(self.input_frame, text = "Browse", cursor = "hand2", command = self.select_input_file)
        self.input_button.configure(bg = "#ffffff", fg = "#000000", font = ("Arial", 12, "bold"), relief = "solid")
        self.input_button.pack(side = "bottom", padx = 3, pady = 3)
        
        # 輸出檔區
        self.output_frame = tk.Frame(self.content, bg = "#474f54")
        self.output_frame.pack(fill = "x", padx = 20, pady = 10)
        self.output_label = tk.Label(self.output_frame, text="Output File", bg = "#474f54", fg = "#ffffff", font = ("Arial", 15, "bold"))
        self.output_label.pack(side = "top", pady = 5)
        self.output_path = tk.StringVar()
        self.output_entry = tk.Entry(self.output_frame, textvariable=self.output_path, font = ("Microsoft JhengHei", 12, "bold"))
        self.output_entry.pack(side = "top", fill = "x", expand = True, padx = 10, pady = 15)

        # 輸出資訊
        self.info_text = tk.StringVar(name = " ")
        self.info_label = tk.Label(self.content, text = self.info_text)
        self.info_label.configure(bg = "#575f64", fg = "#c7e9ff", font = ("Arial", 12, "bold"), justify = "left")
        self.info_label.pack(side = "top", pady = 5)
        
        # 壓縮按鈕區
        self.button_frame = tk.Frame(self.content, bg = "#c7e9ff")
        self.button_frame.pack(side = "bottom", padx = 20, pady = 10)

        # (解)壓縮按鈕
        self.compress_button = tk.Button(self.button_frame, text = "Please Select a File", cursor = "hand2", command = self.operate)
        self.compress_button.configure(bg = "#474f54", fg = "#c7e9ff", font = ("Arial", 15, "bold"), relief = "solid", border = 0)
        self.compress_button.pack(side = "left", padx = 3, pady = 3)

    # 選擇輸入檔
    def select_input_file(self):
        filename = filedialog.askopenfilename()
        self.input_path.set(filename)

        if self.input_path.get() != "":

            if self.input_path.get()[-4:] == ".huf":
                self.output_path.set(filename[:-4])
                self.operation = 0
            else:
                self.output_path.set(filename + ".huf")
                self.operation = 1

            self.compress_button.configure(text = "Compress" if self.operation else "Decompress")

        self.info_label.configure(text = " ")

    # 執行 壓縮/解壓縮
    def operate(self):

        inputfile_path = self.input_path.get()
        outputfile_path = self.output_path.get()

        if inputfile_path == "" or outputfile_path == "":
            self.info_label.configure(fg = "#ffd16e", text = f"Please Select a File !!!")
            return

        command = f"{self.execute_path} {"-c" if self.operation else "-u"} -i {inputfile_path} -o {outputfile_path}"
        status = system(command)

        operation_text = "Compress" if self.operation else "Decompress"

        if status == 0:
            self.info_label.configure(fg = "#a5ff8f", text = f"{operation_text} Successfully!" + self.generateInfo(self.operation))
            print(f"{operation_text} Successfully!")
        else:
            self.info_label.configure(fg = "#ff5c5c", text = f"{operation_text} Failed!")
            print(f"{operation_text} Failed!")

    # 產生執行資訊 
    def generateInfo(self, compress : bool) -> str:
        
        res = "\n"

        if compress:
            originalSize = stat(self.input_path.get()).st_size
            compressSize = stat(self.output_path.get()).st_size

            res += f"Compression Ratio : {100*compressSize/originalSize: .3f}%\n"
            res += f"Original Size : {originalSize} bits\n"
            res += f"Compressed Size : {compressSize} bits"

        else:
            originalSize = stat(self.output_path.get()).st_size
            compressSize = stat(self.input_path.get()).st_size

            res += "\n"
            res += f"Compressed Size : {compressSize} bits\n"
            res += f"Uncompressed Size : {originalSize} bits"

        return res



if __name__ == "__main__":

    root = tk.Tk()
    root.title("Huffman Compression")
    root.iconbitmap("assets\\icon.ico")

    app = HuffmanGUI(root)
    root.mainloop()