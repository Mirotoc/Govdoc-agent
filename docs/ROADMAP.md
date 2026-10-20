# Roadmap

| Phase | Goal | Status |
|---|---|---|
| 0. Concept | Publish the idea, the design, and the label schema | Done |
| 1. Prototype | Text document, rule check, local AI review, saved labels | Done |
| 2. File formats | Read HWPX directly (zipped XML); accept PDF after text extraction | Planned |
| 3. Screen capture | Capture the screen with native OS APIs and pass it to a multimodal model | Planned |
| 4. More rules | Budget totals, missing required fields, per-call template checks | Planned |
| 5. First training | LoRA fine-tuning on 300+ labels, with evaluation | Planned |
| 6. Security audit | A written check that the tool makes no external connections | Planned |
| 7. Packaging | An installer that works without internet access | Planned |

New features keep the same rule: **standard C++ and native OS APIs only.**
Fine-tuning is the one exception. It uses a Python training tool, kept separate from the review program.
