<div align="center">

# 🚦 LogInsight

<img src="imgs/1.png" width="600"/>

### Powerful tool for log analysis and anomaly detection in your applications

<br>
<a href="./LICENSE.md"><img  class="badge" src="https://img.shields.io/github/license/Nighty3098/LogInsight?style=for-the-badge&color=a6e0b8&logoColor=ffffff&labelColor=1c1c29"  height="23px" style="border-radius: 5px;"/></a>
<img class="badge" src="https://img.shields.io/github/issues-pr/Nighty3098/LogInsight?style=for-the-badge&color=ef9f9c&logoColor=85e185&labelColor=1c1c29" height="23px" style="border-radius: 5px;" />
<img class="badge" src="https://img.shields.io/github/release/Nighty3098/LogInsight?style=for-the-badge&color=7589d5&logoColor=ffffff&labelColor=1c1c29" height="23px" style="border-radius: 5px;"/>
<img class="badge" src="https://img.shields.io/github/issues/Nighty3098/LogInsight?style=for-the-badge&color=dbb6ed&logoColor=ffffff&labelColor=1c1c29"  height="23px" style="border-radius: 5px;"/>
<img class="badge" src="https://img.shields.io/github/downloads/Nighty3098/LogInsight/total?style=for-the-badge&color=e0ea9d&logoColor=D9E0EE&labelColor=171b22" height="23px" style="border-radius: 5px;"/>
<img class="badge" src="https://img.shields.io/github/stars/Nighty3098/LogInsight?style=for-the-badge&color=eed49f&logoColor=D9E0EE&labelColor=1c1c29" height="23px" style="border-radius: 5px;"/>
<img class="badge" src="https://img.shields.io/github/forks/Nighty3098/LogInsight?style=for-the-badge&color=9dc3ea&logoColor=D9E0EE&labelColor=1c1c29"  height="23px" style="border-radius: 5px;"/>
<br>
<img src="https://github.com/Nighty3098/DevIcons/blob/main/badges/badges_c.png?raw=true" width="50px" />
<img src="https://github.com/Nighty3098/DevIcons/blob/main/badges/badges_linux.png?raw=true" width="50px" />
<img src="https://github.com/Nighty3098/DevIcons/blob/main/badges/badges_bash.png?raw=true" width="50px" />

<a href="https://github.com/DXS-GROUP/LogInsight/tree/InDev"><kbd> <br>DEV VERSION<br> </kbd></a>

---

## 🚀 Quick Start

```bash
git clone https://github.com/Nighty3098/LogInsight
cd LogInsight
make
sudo ln LogInsight /bin/
LogInsight
```

---

## 📖 Usage

```bash
LogInsight [-r] [-dp] [-h] [-f <level>] -i <file> -fmt <format> [-d <start_date> [<end_date>]] [-strict]
```

| Flag                | Description                                                                                 |
|---------------------|--------------------------------------------------------------------------------------------|
| `-i <file>`         | Path to the log file                                                                       |
| `-f <level>`        | Filter by log level (CRITICAL, WARNING, INFO, DEBUG, etc.)                                 |
| `-r`                | Display all changes in real time                                                           |
| `-fmt <format>`     | Log format (basic, apache, syslog, json)                                                   |
| `-d <start> [end]`  | Filter logs by date/time (format: `YYYY-MM-DD HH:MM:SS`), you can specify a range          |
| `-strict`           | Strict format checking (show only lines matching the format)                               |
| `-dp`               | Do not print log lines                                                                     |
| `-h, --help`        | Show this help                                                                             |

---

## ✨ Features

- **Real-time log monitoring**: Instantly see new log entries as they appear in the file.
- **Flexible log level filtering**: Show only the log levels you care about (e.g., ERROR, WARNING, INFO, DEBUG, etc.).
- **Date/time filtering**: Display logs for a specific period or between two timestamps.
- **Multi-format support**: Parse logs in various formats: basic, Apache, syslog, JSON.
- **Colorful visualization**: Errors (`ERROR`) are highlighted in **red**, successful operations (`SUCCESS`) in **green**, and other lines in default color for easy scanning.
- **Statistics**: Get a summary of log levels (counts for CRITICAL, ERROR, WARNING, INFO, etc.).
- **File size reporting**: See the size of your log file in a human-readable format.
- **Strict format mode**: Only display lines that strictly match the selected log format.
- **Resource usage stats**: Optionally display elapsed time, memory, and CPU usage for log analysis.
- **Easy integration**: Simple CLI interface, can be used in scripts and pipelines.
- **Cross-platform**: Designed for Linux, works in any POSIX environment.

---

## 💡 Example Commands

```bash
# Analyze logs for a specific period
LogInsight -i /var/log/app.log -d "2025-03-12 15:18:06" "2025-03-12 15:18:09"

# Show only errors
LogInsight -i /var/log/app.log -f ERROR

# Real-time monitoring for WARNING and CRITICAL
LogInsight -i /var/log/app.log -f WARNING -f CRITICAL -r

# Use strict format mode
LogInsight -i /var/log/app.log -fmt basic -strict
```

---

## 🎨 Visualization

- Errors (`ERROR`) are highlighted in **red**
- Successes (`SUCCESS`) are highlighted in **green**
- All other lines are shown in the default color

<img src="imgs/2.png" width="600"/>
<img src="imgs/3.png" width="600"/>
<img src="imgs/4.png" width="600"/>
<img src="imgs/5.png" width="600"/>

---

## 🛡️ License

This project is licensed under the [MIT License](./LICENSE.md).

---

**LogInsight** — your fast and convenient way to analyze logs and detect anomalies!

</div>
