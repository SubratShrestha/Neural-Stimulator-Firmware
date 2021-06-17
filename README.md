# Neural-Stimulator-Firmware
Repository for the Neural Stimulator VIP.

`/old` contains:
* STM 32 code and notes for last year's project.

`/new` contains:
* PSOC 6 code and notes for the current project.


## Contribute
* To contribute, please make a copy of the repository with:
  * `git clone <SSH or HTTPS>`
* Make a new branch:
  * on the github page: `https://github.com/SubratShrestha/Neural-Stimulator-Firmware/`
    * head over to the branches dropdown and type in the new branch name and the base branch and create one
  * or in a terminal with `git checkout -b "New branch"`
  * or in VSCode -> branches on bottom right -> Create new Branch
* Checkout into this branch on the terminal with: `git fetch && git checkout "New branch"`
* Stage and commit your changes, and push with `git push origin "New branch"` or the little wheel in VSCode
* Once you're done working on the new branch, make a Pull Request:
  * on the github page on the branch you created, head to "Pull request" and make a new one.
  * this is the same as a "Merge Request" on GitLab.


All of this stuff is done so that we don't ever push to the master branch. If this is done and an issue comes up, it will be a nightmare to go through and find the exact commit that caused the issue and fix it.
With these instructions, a Pull Request gets approved by someone so the code is at least checked on and since there are generally less merges than commits, it will be much easier to tract an issue down if it comes to it.
  