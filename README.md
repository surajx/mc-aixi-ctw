# MC-AIXI-CTW 

An implementation of the MC-AIXI-CTW [paper](docs/mc_aixi_ctw__a_AIXI_approximation.pdf). The paper provides an approximation of the [AIXI](http://www.hutter1.net/ai/uaibook.htm) model. View the detailed report of the project [here](docs/report/AIXI_project_report.pdf).

#### Linux/Mac OS
Extract the source files into a folder and run:
```bash
make all
./bin/aixi -c conf/coinflip.conf
```

#### Usage:
```bash
./bin/aixi -c domain_config_file [-x cross_domain_name] [-l log_file_prefix]
./bin/aixi -h
-h : Prints the Help Message
-c <file>: Tells AIXI which domain and configuration to use. (Mandatory)
-x <file>: Run evaluation with a different domain. (Optional)
-l <log file prefix>: Give a prefix to the output log file. Useful when running multiple trials. (Optional)
```

Available Domains: coin-flip, extended-tiger, tictactoe, biased-rock-paper-scissor, kuhn-poker, pacman.
