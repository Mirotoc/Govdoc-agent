# Compliance Checklist

> This is a developer's reference checklist, **not legal advice.**
> Laws and guidelines change often. Check the original texts on the [Korean Law Information Center](https://www.law.go.kr) and the relevant ministries' notices.
> Any real deployment needs a review by the institution's security officer and legal team.

<br>

## Korean laws and guidelines

| Reference | Where it applies | How the design responds |
|---|---|---|
| Personal Information Protection Act | Names, contact details, and resident registration numbers in documents | Processed only on the PC and masked before reaching the model. Retention and deletion rules to be defined |
| PIPC Pseudonymization Guidelines | Using label data for training | Document the pseudonymization rules and check re-identification risk |
| AI Basic Act (in force since 2026-01-22) and its Enforcement Decree | Transparency and safety of AI output | Every run says the findings are AI suggestions. Record whether the tool counts as high-impact AI |
| MSIT AI guidelines (transparency, safety, impact assessment) | Same as above | A clause-by-clause response table is planned |
| NIS National Network Security Framework (N2SF) | Use inside a public institution's work network | Confirm the data classification level and run only inside a network of that level |
| Open-source licenses | Models and tools | The model is Apache-2.0 (Qwen3.5). The code uses no third-party libraries |

<br>

## International standards

| Standard | Scope | Application |
|---|---|---|
| ISO/IEC 27001 | Information security management | Access control, encryption, logging, asset management |
| ISO/IEC 27701 | Privacy information management | Personal data flow and legal basis for processing |
| ISO/IEC 42001 | AI management systems | AI use policy, roles and responsibilities, model version control |
| ISO/IEC 23894 | AI risk management | False positives, false negatives, and over-reliance on AI output |
| ISO/IEC 5259 | Data quality for AI | Label agreement, data provenance, and versioning |

<br>

## Checklist by phase

**Design**
- [x] Data flow diagram
- [x] Zero egress fixed in code (`127.0.0.1`)
- [x] Model license checked

**Development**
- [x] Personal data masked before the model call
- [x] Labels record their source and a timestamp
- [ ] Reviewer identity and audit log
- [ ] Pre-commit check that blocks real documents

**Training data**
- [ ] Pseudonymization rules documented
- [ ] Retention period and deletion procedure

**Before deployment**
- [ ] Institutional security review
- [ ] Check whether a privacy impact assessment is required
- [ ] Record the high-impact AI assessment
- [ ] User notice: AI findings are advisory, and responsibility stays with the reviewer
