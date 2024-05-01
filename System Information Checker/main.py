import sys
import subprocess
import numpy as np
from datetime import datetime
from PyQt5.QtWidgets import QApplication, QMainWindow, QPushButton, QVBoxLayout, QWidget, QGridLayout, QLabel, QAction
from PyQt5.QtGui import QFont
import psutil
from PyQt5.QtCore import QTimer
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

class SystemInfoApp(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("System Information")
        self.setGeometry(100, 100, 800, 600)

        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)
        self.layout = QVBoxLayout()
        self.central_widget.setLayout(self.layout)

        self.buttons_layout = QVBoxLayout()
        self.graph_layout = QVBoxLayout()
        self.layout.addLayout(self.buttons_layout)
        self.layout.addLayout(self.graph_layout)

        self.buttons = {
            "System Info": ["Boot Time", "CPU Info", "Memory Info", "Swap Memory", "Disk Info", "Network Info", "Health Info", "Temperature Info"],
            "Monitoring": ["CPU Utilization", "Memory Usage", "Disk Usage"],
            "Diagnostics": ["System Health and Diagnostics"],
            "Advanced Monitoring": ["Advanced CPU Info", "Advanced Memory Info", "Advanced Disk Info", "Advanced Network Info", "Advanced GPU Monitoring", "Comprehensive Sensor Monitoring", "Network Monitoring and Diagnostics", "Power and Battery Monitoring", "System Resource Monitoring", "System Benchmarking and Performance Testing", "System Optimization and Tuning", "Advanced Logging and Reporting", "Remote Monitoring and Management"]
        }

        self.add_buttons_to_layout()
        self.create_menu()

        self.figure = Figure(figsize=(8, 5))
        self.canvas = FigureCanvas(self.figure)
        self.graph_layout.addWidget(self.canvas)

        self.setup_timer()

    def setup_timer(self):
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_graphs)
        self.timer.start(1000)

    def create_menu(self):
        menubar = self.menuBar()
        file_menu = menubar.addMenu('File')
        exit_action = QAction('Exit', self)
        exit_action.triggered.connect(self.close)
        file_menu.addAction(exit_action)

    def add_buttons_to_layout(self):
        for category, buttons in self.buttons.items():
            self.buttons_layout.addWidget(QLabel(category, font=QFont('Arial', 14, QFont.Bold)))
            button_grid_layout = QGridLayout()
            row = 0
            col = 0
            for btn_text in buttons:
                btn_obj = QPushButton(btn_text, self)
                btn_obj.clicked.connect(self.show_info)
                btn_obj.setStyleSheet("background-color: #2E8B57; color: white; font-size: 14px; padding: 10px 20px;")
                button_grid_layout.addWidget(btn_obj, row, col)
                col += 1
                if col > 2:
                    col = 0
                    row += 1
            self.buttons_layout.addLayout(button_grid_layout)

    def show_info(self):
        sender = self.sender()
        option = sender.text()
        self.figure.clear()
        ax = self.figure.add_subplot(111)

        try:
            if option == "System Health and Diagnostics":
                try:
                    hardware_diagnosis_output = subprocess.check_output(["diagnose_hardware_tool"])
                    ax.text(0.5, 0.5, f"Hardware Diagnosis: {hardware_diagnosis_output.decode('utf-8')}",
                            horizontalalignment='center', verticalalignment='center', fontsize=12)
                except FileNotFoundError:
                    ax.text(0.5, 0.5, "Error: Hardware diagnostic tool not found.",
                            horizontalalignment='center', verticalalignment='center', fontsize=12)
                except subprocess.CalledProcessError:
                    ax.text(0.5, 0.5, "Error: Unable to perform hardware diagnostics.",
                            horizontalalignment='center', verticalalignment='center', fontsize=12)
        except Exception as e:
            ax.text(0.5, 0.5, f"Error: {e}", horizontalalignment='center', verticalalignment='center', fontsize=12)

        self.canvas.draw()

        if option == "Boot Time":
            boot_time_timestamp = psutil.boot_time()
            bt = datetime.fromtimestamp(boot_time_timestamp)
            ax.text(0.5, 0.5, f"Boot Time: {bt.strftime('%Y/%m/%d %H:%M:%S')}",
                    horizontalalignment='center', verticalalignment='center', fontsize=12)

        elif option == "CPU Info":
            cpu_count_logical = psutil.cpu_count(logical=True)
            cpu_count_physical = psutil.cpu_count(logical=False)
            cpufreq = psutil.cpu_freq()
            total_cpu_usage = psutil.cpu_percent()
            ax.text(0.5, 0.5, f"Physical Cores: {cpu_count_physical}\n"
                              f"Total Cores: {cpu_count_logical}\n"
                              f"Max Frequency: {cpufreq.max:.2f}Mhz\n"
                              f"Min Frequency: {cpufreq.min:.2f}Mhz\n"
                              f"Current Frequency: {cpufreq.current:.2f}Mhz\n"
                              f"Total CPU Usage: {total_cpu_usage}%",
                    horizontalalignment='center', verticalalignment='center', fontsize=12)

        elif option == "Memory Info":
            svmem = psutil.virtual_memory()
            ax.bar(["Total Memory", "Used Memory", "Available Memory"],
                   [svmem.total, svmem.used, svmem.available])
            ax.set_ylabel('Memory (Bytes)')
            ax.set_title('Memory Usage')

        elif option == "Swap Memory":
            swap = psutil.swap_memory()
            ax.bar(["Total Swap Memory", "Used Swap Memory", "Free Swap Memory"],
                   [swap.total, swap.used, swap.free], color='red')
            ax.set_ylabel('Memory (Bytes)')
            ax.set_title('Swap Memory Usage')

        elif option == "Disk Info":
            disk_partitions = psutil.disk_partitions()
            disk_labels = []
            disk_sizes = []
            for partition in disk_partitions:
                try:
                    partition_usage = psutil.disk_usage(partition.mountpoint)
                    disk_labels.append(partition.device)
                    disk_sizes.append(partition_usage.total)
                except PermissionError:
                    continue
            ax.pie(disk_sizes, labels=disk_labels, autopct='%1.1f%%', startangle=90)
            ax.axis('equal')
            ax.set_title('Disk Usage')

        elif option == "Network Info":
            net_io = psutil.net_io_counters()
            ax.bar(["Bytes Sent", "Bytes Received"],
                   [net_io.bytes_sent, net_io.bytes_recv], color='purple')
            ax.set_ylabel('Bytes')
            ax.set_title('Network Traffic')

        elif option == "Health Info":
            battery_status = psutil.sensors_battery().power_plugged if hasattr(psutil, 'sensors_battery') and psutil.sensors_battery() else "N/A"
            ups_status = psutil.sensors_ups().power_plugged if hasattr(psutil, 'sensors_ups') and psutil.sensors_ups() else "N/A"
            ax.bar(["Battery", "UPS"], [battery_status, ups_status], color='green')
            ax.set_ylabel('Health Status')
            ax.set_title('System Health')

        elif option == "Temperature Info":
            temperatures = psutil.sensors_temperatures()
            cpu_temp = temperatures['coretemp'] if 'coretemp' in temperatures else None
            gpu_temp = temperatures['amdgpu'] if 'amdgpu' in temperatures else None
            ax.bar(["CPU", "GPU"], [cpu_temp, gpu_temp], color='orange')
            ax.set_ylabel('Temperature (°C)')
            ax.set_title('System Temperature')


        elif option == "CPU Utilization":
            times = list(range(10))
            usage_per_core = np.random.rand(10, psutil.cpu_count())
            for core, usage in enumerate(usage_per_core.T):
                ax.plot(times, usage, label=f'Core {core}')
            ax.legend()
            ax.set_xlabel('Time')
            ax.set_ylabel('CPU Usage (%)')
            ax.set_title('CPU Utilization Over Time')

        elif option == "Memory Usage":
            labels = ['Total Memory', 'Used Memory', 'Available Memory']
            sizes = np.random.randint(1, 100, size=3)
            ax.bar(labels, sizes)
            ax.set_ylabel('Memory (Bytes)')
            ax.set_title('Memory Usage')

        elif option == "Disk Usage":
            partitions = psutil.disk_partitions()
            disk_sizes = []
            disk_labels = []
            for partition in partitions:
                try:
                    partition_usage = psutil.disk_usage(partition.mountpoint)
                    disk_labels.append(partition.device)
                    disk_sizes.append(partition_usage.total)
                except PermissionError:
                    continue
            ax.pie(disk_sizes, labels=disk_labels, autopct='%1.1f%%', startangle=90)
            ax.axis('equal')
            ax.set_title('Disk Usage')

        elif option == "System Health and Diagnostics":
            # Perform hardware diagnostics
            try:
                hardware_diagnosis_output = subprocess.check_output(["diagnose_hardware_tool"])
                ax.text(0.5, 0.5, f"Hardware Diagnosis: {hardware_diagnosis_output.decode('utf-8')}",
                        horizontalalignment='center', verticalalignment='center', fontsize=12)
            except subprocess.CalledProcessError:
                ax.text(0.5, 0.5, "Error: Unable to perform hardware diagnostics.",
                        horizontalalignment='center', verticalalignment='center', fontsize=12)

        elif option == "Advanced CPU Monitoring":
            # Monitor real-time CPU usage, frequency, and temperature
            cpu_usage = psutil.cpu_percent(interval=1, percpu=True)
            cpu_freq = psutil.cpu_freq()
            cpu_temp = psutil.sensors_temperatures()['coretemp'] if 'coretemp' in psutil.sensors_temperatures() else None

            ax.plot(cpu_usage, label='CPU Usage (%)')
            ax.plot([cpu_freq.current] * len(cpu_usage), label='CPU Frequency (MHz)')
            if cpu_temp:
                ax.plot([cpu_temp[0].current] * len(cpu_usage), label='CPU Temperature (°C)')
            ax.legend()
            ax.set_xlabel('Time')
            ax.set_ylabel('Value')
            ax.set_title('Advanced CPU Monitoring')


        elif option == "Detailed Memory Monitoring":
            # Monitor real-time memory usage and performance metrics
            mem = psutil.virtual_memory()
            swap = psutil.swap_memory()

            ax.plot([mem.used, mem.available, swap.used], label=['Used Memory', 'Available Memory', 'Swap Used'])
            ax.legend()
            ax.set_xlabel('Time')
            ax.set_ylabel('Memory (Bytes)')
            ax.set_title('Detailed Memory Monitoring')


        elif option == "Comprehensive Disk Monitoring":
            # Monitor real-time disk usage, health, and performance
            disk_io = psutil.disk_io_counters(perdisk=True)
            partitions = psutil.disk_partitions()

            for partition in partitions:
                try:
                    usage = psutil.disk_usage(partition.mountpoint)
                    ax.plot([usage.used, usage.free], label=[f'{partition.mountpoint} Used', f'{partition.mountpoint} Free'])
                except PermissionError:
                    continue
            ax.legend()
            ax.set_xlabel('Time')
            ax.set_ylabel('Disk Space (Bytes)')
            ax.set_title('Comprehensive Disk Monitoring')


        elif option == "Advanced GPU Monitoring":
            # Monitor real-time GPU usage, temperature, and performance
            try:
                import pynvml
                pynvml.nvmlInit()
                handle = pynvml.nvmlDeviceGetHandleByIndex(0)
                gpu_info = pynvml.nvmlDeviceGetUtilizationRates(handle)
                gpu_temp = pynvml.nvmlDeviceGetTemperature(handle, pynvml.NVML_TEMPERATURE_GPU)
                ax.plot([gpu_info.gpu, gpu_info.memory], label=['GPU Usage (%)', 'Memory Usage (%)'])
                ax.plot([gpu_temp] * 2, label='GPU Temperature (°C)')
                ax.legend()
                ax.set_xlabel('Time')
                ax.set_ylabel('Value')
                ax.set_title('Advanced GPU Monitoring')
            except Exception as e:
                ax.text(0.5, 0.5, f"Error: {e}", horizontalalignment='center', verticalalignment='center', fontsize=12)


        elif option == "Comprehensive Sensor Monitoring":
            # Monitor temperatures, fan speeds, and voltage readings
            try:
                sensors = psutil.sensors_temperatures()
                fan_speeds = psutil.sensors_fans()
                voltage_readings = psutil.sensors_battery()
                
                # Plot temperature
                for sensor, temps in sensors.items():
                    for temp in temps:
                        ax.plot([temp.current] * 2, label=f'{sensor} Temperature (°C)')
                        
                # Plot fan speeds
                for fan, speed in fan_speeds.items():
                    ax.plot([speed] * 2, label=f'{fan} Speed (RPM)')
                
                # Plot voltage readings
                if voltage_readings:
                    ax.plot([voltage_readings.voltage] * 2, label='Battery Voltage (V)')
                
                ax.legend()
                ax.set_xlabel('Time')
                ax.set_ylabel('Value')
                ax.set_title('Comprehensive Sensor Monitoring')
            except Exception as e:
                ax.text(0.5, 0.5, f"Error: {e}", horizontalalignment='center', verticalalignment='center', fontsize=12)


        elif option == "Network Monitoring and Diagnostics":
            # Monitor real-time network usage and perform network diagnostics
            try:
                net_io = psutil.net_io_counters(pernic=True)
                for interface, stats in net_io.items():
                    ax.plot([stats.bytes_sent, stats.bytes_recv], label=[f'{interface} Sent', f'{interface} Received'])
                ax.legend()
                ax.set_xlabel('Time')
                ax.set_ylabel('Bytes')
                ax.set_title('Network Monitoring and Diagnostics')
            except Exception as e:
                ax.text(0.5, 0.5, f"Error: {e}", horizontalalignment='center', verticalalignment='center', fontsize=12)


        elif option == "Power and Battery Monitoring":
            # Monitor power consumption, battery status, and health
            try:
                battery = psutil.sensors_battery()
                if battery:
                    ax.bar(["Battery Percent", "Battery Power Plugged"], [battery.percent, battery.power_plugged], color='blue')
                    ax.set_ylabel('Battery Status')
                    ax.set_title('Power and Battery Monitoring')
                else:
                    ax.text(0.5, 0.5, "No battery found.", horizontalalignment='center', verticalalignment='center', fontsize=12)
            except Exception as e:
                ax.text(0.5, 0.5, f"Error: {e}", horizontalalignment='center', verticalalignment='center', fontsize=12)


        elif option == "System Resource Monitoring":
            # Monitor system resource usage for processes and applications
            try:
                processes = psutil.process_iter(['pid', 'name', 'cpu_percent', 'memory_percent'])
                process_info = [(p.info['name'], p.info['cpu_percent'], p.info['memory_percent']) for p in processes]
                
                # Display top 5 processes by CPU and memory usage
                sorted_processes_cpu = sorted(process_info, key=lambda x: x[1], reverse=True)[:5]
                sorted_processes_mem = sorted(process_info, key=lambda x: x[2], reverse=True)[:5]
                
                ax.bar([f"Process {i+1}" for i in range(5)], [p[1] for p in sorted_processes_cpu], label='CPU Usage (%)')
                ax.bar([f"Process {i+1}" for i in range(5)], [p[2] for p in sorted_processes_mem], label='Memory Usage (%)')
                ax.legend()
                ax.set_xlabel('Processes')
                ax.set_ylabel('Usage (%)')
                ax.set_title('System Resource Monitoring')
            except Exception as e:
                ax.text(0.5, 0.5, f"Error: {e}", horizontalalignment='center', verticalalignment='center', fontsize=12)


        elif option == "System Benchmarking and Performance Testing":
            # Perform system benchmarking and performance testing
            try:
                import time
                start_time = time.time()
                # Simulate CPU-intensive task
                result = 0
                for i in range(10**7):
                    result += i
                elapsed_time = time.time() - start_time
                
                ax.text(0.5, 0.5, f"CPU Performance Benchmark\nResult: {result}\nElapsed Time: {elapsed_time:.2f} seconds",
                        horizontalalignment='center', verticalalignment='center', fontsize=12)
                ax.set_title('System Benchmarking and Performance Testing')
            except Exception as e:
                ax.text(0.5, 0.5, f"Error: {e}", horizontalalignment='center', verticalalignment='center', fontsize=12)


        elif option == "System Optimization and Tuning":
            # Provide options for optimizing system performance
            optimization_options = [
                "Adjust CPU Governor Settings",
                "Tune Disk I/O Scheduler",
                "Optimize Memory Allocation",
                "Fine-tune Network Parameters"
            ]
            ax.clear()
            ax.set_title('System Optimization and Tuning Options')
            ax.set_xticks([])
            ax.set_yticks([])
            for i, opt in enumerate(optimization_options, start=1):
                ax.text(0.5, 1-0.1*i, f"{i}. {opt}", horizontalalignment='center', verticalalignment='center', fontsize=12)

        elif option == "Advanced Logging and Reporting":
            # Enable logging and generate comprehensive reports
            logging_options = [
                "Enable Verbose System Logging",
                "Log Kernel Events",
                "Log Application Events",
                "Customize Log Rotation Policies"
            ]
            ax.clear()
            ax.set_title('Advanced Logging and Reporting Options')
            ax.set_xticks([])
            ax.set_yticks([])
            for i, opt in enumerate(logging_options, start=1):
                ax.text(0.5, 1-0.1*i, f"{i}. {opt}", horizontalalignment='center', verticalalignment='center', fontsize=12)
        
        elif option == "Remote Monitoring and Management":
            # Provide options for remote monitoring and management
            remote_options = [
                "Setup SSH with Key-based Authentication",
                "Deploy Monitoring Agents on Remote Systems",
                "Implement Remote Script Execution",
                "Configure Firewall for Secure Remote Access"
            ]
            ax.clear()
            ax.set_title('Remote Monitoring and Management Options')
            ax.set_xticks([])
            ax.set_yticks([])
            for i, opt in enumerate(remote_options, start=1):
                ax.text(0.5, 1-0.1*i, f"{i}. {opt}", horizontalalignment='center', verticalalignment='center', fontsize=12)



        self.canvas.draw()

    def update_graphs(self):
        # Update the graphs dynamically (e.g., for real-time monitoring)
        pass

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = SystemInfoApp()
    window.show()
    sys.exit(app.exec_())
