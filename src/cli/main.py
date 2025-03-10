import sys
sys.path.append("/app/build/lib")  # Add parent directory to path
import bindings
import argparse

def main():
    parser = argparse.ArgumentParser(description="UwU FileShare CLI")
    parser.add_argument("command", choices=["start", "connect", "send", "test"], help="Command to execute")
    parser.add_argument("args", nargs="*", help="Arguments for the command")
    args = parser.parse_args()

    peer = bindings.PeerNode(5000)  # Start node on port 5000

    if args.command == "start":
        peer.start()
    
    elif args.command == "connect":
        if len(args.args) < 2:
            print("Usage: main.py connect <IP> <PORT>")
            return
        ip, port = args.args
        peer.connectToPeer(ip, int(port))
    
    elif args.command == "send":
        if len(args.args) < 1:
            print("Usage: main.py send <MESSAGE>")
            return
        message = " ".join(args.args)
        peer.send_message(message)

    elif args.command == "test":
        peer.start()
        peer.connectToPeer("localhost", 5000)
        peer.sendToPeer("Hello from 5000")


if __name__ == "__main__":
    main()
