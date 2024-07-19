# Setting up forked OpenROAD modules
Please fork ```OpenROAD-flow-scripts``` and ```OpenROAD```
## Clone the forked OpenROAD-flow-scripts
```
git clone --recursive git@github.com:rpriyadarshi/OpenROAD-flow-scripts.git
```
## Move the tags to master
```
git submodule foreach 'git checkout master'
git submodule foreach 'git pull origin master'
```
## Just in case master was actually main
```
git submodule foreach 'git checkout main'
git submodule foreach 'git pull origin main'
```
## Commit the files and push to master
```
git add .
git commit -m "Updated submodules to latest master"
git push
```
# Add submodule to OpenROAD
```
cd tools/OpenROAD/
git submodule add ../OpenROAD-modules.git src/prv
git submodule update --init --recursive
```
## Checkin the changes
```
git add .gitmodules src/prv 
git commit -m "Added submodule to src/prv"
git push
```
## Checkin the changes at OpenROAD-flow-scripts
```
cd ../../
git add .
git commit -m "Added submodule to OpenROAD"
git push
```
