# Whisperly — Truly Private Messenger  
**Privacy is a right, not a privilege.**  

---

### Why Whisperly?  
We grew tired of the illusion of privacy. Most messengers require phone numbers, store metadata, or rely on centralized servers that can be compromised (yes, even Signal). **Whisperly** is the answer. No accounts, no tracking, no compromises. Just you, your contact, and encryption you can trust.  

This project was born for personal use, but if it helps others too, we’ll be glad.  

---

### Core Principles  
**Privacy by Design**  
- **No Phone Numbers or Tracking**: Unique cryptographic keys replace phone numbers.  
- **Zero Metadata Leaks**: Servers act as dumb relays — they don’t know who’s communicating or what’s being sent.  
- **Forward Secrecy**: Session keys are ephemeral and destroyed after use.  

**Security First**  
- **TLS 1.3 + End-to-End Encryption**: Protocol-level encryption with no "custom" implementations.  
- **Anti-Feature Philosophy**: No cloud backups, third-party integrations, or convenience trade-offs.  

**Simplicity & Transparency**  
- **Minimalist Design**: Chat without distractions — no ads, bloat, or feature creep.  
- **Fully Open Source**: Audit every line of code. No hidden backdoors.  

---

### Architecture
[Client] <---(TLS 1.3)---> [Server] <---(TLS 1.3)---> [Client]

---

### Installation (Coming Soon)  
While we prepare the release, you can:  
1. Clone the repository:  
   ```bash  
   git clone https://github.com/lnewtium/whispernet.git  
2. Follow updates in the dev branch.
3. Want to help? See the Contributing section below.

---

### Contributing  
We believe in open source as a philosophy. Your contributions matter:  
- Found a bug? Open an issue.  
- Have an idea? Discuss it in Discussions.  
- Pull requests are welcome!  

**Guidelines**:  
- Keep code simple and readable.   
- No unnecessary dependencies.  

---

Use the code freely, but remember: **privacy is not for sale**.
