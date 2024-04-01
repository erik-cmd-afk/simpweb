# Instructions for running simpweb as a background service

### To run `simpweb` as a background service, we can use a process manager like `nssm`

## 1. Download "nssm" 

- Download `nssm` from `https://nssm.cc/download`

### [Download nssm](https://nssm.cc/download)

![plot](./resources/png/nssm.download.png)

## 2. Extract the `nssm.zip` file

![plot](./resources/png/nssm.extract.png)

## 3. Open the `nssm-<version>`folder and go to `win64` or `win32` *

### *depending on your Windows bit system

![plot](./resources/png/nssm.go.to.png)

## 4. Open a new terminal in this folder

![plot](./resources/png/nssm.new.terminal.png)

## 6. A new service 

### Enter this command: `.\nssm install simpweb`

![plot](./resources/png/nssm.enter.command.png)

## 7. Now will open a new window 

- ignore the window, we'll need it soon

## 8. Download `simpweb.zip` from GitHub