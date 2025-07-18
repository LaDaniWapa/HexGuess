#!/bin/bash
# 3DS FTP Deployment Script (Bash version)

# Configuration (edit these values)
FTP_HOST="192.168.1.26"
FTP_USER="3ds"
FTP_PASS="3ds"
FTP_PORT="5000"
PROJECT_NAME="HexGuess"
BUILD_DIR="build"

# FTP paths on 3DS
FTP_3DSX_PATH="/3ds/"
FTP_CIA_PATH="/cia/"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠ $1${NC}"
}

print_info() {
    echo -e "$1"
}

# Function to upload file via FTP
upload_file() {
    local local_file="$1"
    local remote_path="$2"
    local remote_file="$3"

    if [ ! -f "$local_file" ]; then
        print_error "File not found: $local_file"
        return 1
    fi

    print_info "Uploading $local_file → $remote_path$remote_file"

    # Use curl for FTP upload
#    if curl -T "$local_file" "ftp://$FTP_USER:$FTP_PASS@$FTP_HOST:$FTP_PORT$remote_path$remote_file" --create-dirs -s; then
    if curl -T "$local_file" "ftp://$FTP_HOST:$FTP_PORT$remote_path$remote_file" --create-dirs -s; then
        print_success "Uploaded $remote_file"
        return 0
    else
        print_error "Failed to upload $remote_file"
        return 1
    fi
}

# Function to check if FTP server is reachable
check_connection() {
    print_info "Checking connection to $FTP_HOST:$FTP_PORT..."

#    if curl -s "ftp://$FTP_USER:$FTP_PASS@$FTP_HOST:$FTP_PORT/" > /dev/null 2>&1; then
    if curl -s "ftp://$FTP_HOST:$FTP_PORT/" > /dev/null 2>&1; then
        print_success "Connected to 3DS FTP server"
        return 0
    else
        print_error "Failed to connect to 3DS FTP server"
        print_info "Make sure:"
        print_info "  - Your 3DS is on and connected to WiFi"
        print_info "  - FBI or ftpd is running on your 3DS"
        print_info "  - The IP address is correct: $FTP_HOST"
        return 1
    fi
}

# Function to deploy 3DSX file
deploy_3dsx() {
    local file="$BUILD_DIR/${PROJECT_NAME}.3dsx"
    upload_file "$file" "$FTP_3DSX_PATH" "${PROJECT_NAME}.3dsx"
}

# Function to deploy CIA file
deploy_cia() {
    local file="$BUILD_DIR/${PROJECT_NAME}.cia"
    upload_file "$file" "$FTP_CIA_PATH" "${PROJECT_NAME}.cia"
}

# Function to show usage
show_usage() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  --3dsx      Deploy only 3DSX file"
    echo "  --cia       Deploy only CIA file"
    echo "  --all       Deploy both files (default)"
    echo "  --build     Build before deploying"
    echo "  --config    Show current configuration"
    echo "  --help      Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                    # Deploy both files"
    echo "  $0 --3dsx            # Deploy only 3DSX"
    echo "  $0 --build --all     # Build then deploy both"
}

# Function to show configuration
show_config() {
    echo "=== Current Configuration ==="
    echo "FTP Host: $FTP_HOST"
    echo "FTP User: $FTP_USER"
    echo "FTP Port: $FTP_PORT"
    echo "Project Name: $PROJECT_NAME"
    echo "Build Directory: $BUILD_DIR"
    echo "3DSX Path: $FTP_3DSX_PATH"
    echo "CIA Path: $FTP_CIA_PATH"
    echo ""
    echo "Edit this script to change these values."
}

# Function to build project
build_project() {
    print_info "Building project..."

    if [ ! -d "$BUILD_DIR" ]; then
        mkdir -p "$BUILD_DIR"
    fi

    cd "$BUILD_DIR" || exit 1

    if cmake .. && make; then
        print_success "Build completed"
        cd ..
        return 0
    else
        print_error "Build failed"
        cd ..
        return 1
    fi
}

# Main script
main() {
    local deploy_3dsx=false
    local deploy_cia=false
    local do_build=false

    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            --3dsx)
                deploy_3dsx=true
                shift
                ;;
            --cia)
                deploy_cia=true
                shift
                ;;
            --all)
                deploy_3dsx=true
                deploy_cia=true
                shift
                ;;
            --build)
                do_build=true
                shift
                ;;
            --config)
                show_config
                exit 0
                ;;
            --help)
                show_usage
                exit 0
                ;;
            *)
                echo "Unknown option: $1"
                show_usage
                exit 1
                ;;
        esac
    done

    # Default to deploying both if no specific option given
    if [ "$deploy_3dsx" = false ] && [ "$deploy_cia" = false ]; then
        deploy_3dsx=true
        deploy_cia=true
    fi

    # Build if requested
    if [ "$do_build" = true ]; then
        if ! build_project; then
            exit 1
        fi
    fi

    # Check connection
    if ! check_connection; then
        exit 1
    fi

    # Deploy files
    local success=true

    if [ "$deploy_3dsx" = true ]; then
        if ! deploy_3dsx; then
            success=false
        fi
    fi

    if [ "$deploy_cia" = true ]; then
        if ! deploy_cia; then
            success=false
        fi
    fi

    # Final result
    if [ "$success" = true ]; then
        print_success "🎉 Deployment successful!"
        exit 0
    else
        print_error "❌ Deployment failed!"
        exit 1
    fi
}

# Run main function with all arguments
main "$@"