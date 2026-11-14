# Phase 1.2 implementation checklist

## Confirmed from the team's Phase 1.1 report and recommended-DS sheet

- [x] Preserve pointer-based car and bay lists.
- [x] Use the lab-provided data structures, or derive from them; do not use STL containers.
- [x] Keep the Phase 1.1 list choices and add their required project-specific operations.
- [x] Cars in diagnosis/service must hold a pointer to their assigned bay.
- [x] Implement the three waiting-service queues with a running total of waiting service time.
- [ ] Implement and test the full simulator and all required operations.

## Required source material before implementation

1. Official Phase 1.2 assignment/specification (the complete text, not only a link embedded in a Word file).
2. Appendix A with the exact required data members and class interfaces.
3. Course lab header/source files for `LinkedQueue`, `PriQueue`, and `ArrayStack` (plus any base classes they depend on).
4. Any required sample input/output files and the compiler/IDE requirements.

The Google Docs URLs embedded in the uploaded Word document could not be read by the available document access. Upload/export the documents as PDF or DOCX, or paste their contents here.

## Implementation and verification gates

- [ ] Match every Appendix A data member, constructor, destructor, and method signature exactly.
- [ ] Confirm the actual lab APIs before deriving or calling their methods.
- [ ] Implement the project-specific list operations without changing the supplied lab structures.
- [ ] Ensure ownership and deletion of dynamically allocated cars/bays are consistent and leak-free.
- [ ] Implement stream output and required print/count operations exactly as specified.
- [ ] Add a reproducible simulator with at least 1,000 generated IDLE cars if that is required by the official Phase 1.2 sheet.
- [ ] Compile with the target compiler and fix all errors/warnings that affect correctness.
- [ ] Run deterministic tests for empty, one-element, many-element, boundary, random-removal, and priority-order cases.
- [ ] Record actual build and test results; do not mark tests passed unless they were run.
